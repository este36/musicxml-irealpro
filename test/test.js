import { readFileSync } from 'fs';
import * as mxl2irp from '../bindings/mxl2irp.js';
import { WasmString } from '../bindings/mxl2irp.js';
import * as fflate from './vendors/fflate-0.8.2/lib/index.cjs';

function is_mxl_file(f)
{
	const len = f.len;
	return (len > 4
			&& f[len - 4] === '.'
			&& f[len - 3] === 'm'
			&& f[len - 2] === 'x'
			&& f[len - 1] === 'l');
}

function get_song_from_path(path)
{
	try {
		const file = new WasmString(readFileSync(path, 'utf8'));
		const irp_song = mxl2irp.parse_musicxml(file.buf, file.len);
		mxl2irp.free(file.buf);
		return irp_song != 0 ? irp_song : null;
	} catch (err) {
		console.error(err);
		return null;
	}
}

function print_url(url)
{
	console.log(`<h1>${url}</h1>`);
}

function main(argc, argv)
{
	if (argc === 1) {
		console.error(`Usage: ${argv[0]} (1 or more)[path-to-musicxml]`);
		return 1;
	}

	if (argc === 2) {
		const irp_song = get_song_from_path(argv[1]);
		if (irp_song === null) {
			console.error(`${argv[1]}: PARSE SONG FAIL`);
			return 1;
		}
		const url = mxl2irp.irp_song_get_html(irp_song);
		if (url === null) {
			console.error(`HTML RENDER FAIL`);
			return 1;
		}
		print_url(url);
		mxl2irp.irp_song_free(irp_song);
		mxl2irp.free(url);
	} else {
		let irp_playlist = mxl2irp.irp_playlist_create("Test");
		for (let i = 1; i < argc; i++) {
			const irp_song = get_song_from_path(argv[i]);
			if (irp_song === null) {
				mxl2irp.irp_playlist_free(irp_playlist);
				console.error(`${argv[i]}: PARSE SONG FAIL`);
				return 1;
			}
			mxl2irp.irp_playlist_append(irp_playlist, irp_song);
		}
		const url = mxl2irp.irp_playlist_get_html(irp_playlist);
		if (url === null) {
			console.error(`HTML RENDER FAIL`);
			return 1;
		}
		print_url(url);
		mxl2irp.irp_playlist_free(irp_playlist);
		mxl2irp.free(url);
	}
	return 0;
}

await mxl2irp.initWasm();
main(process.argv.length - 1, process.argv.slice(1));
