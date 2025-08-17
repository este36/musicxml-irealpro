import { readFileSync } from 'fs';
import * as mxl2irp from '../bindings/mxl2irp.js';
import * as fflate from './vendors/fflate-0.8.2/lib/index.cjs';

function is_mxl_file(f)
{
	const len = f.length;
	return (len > 4
			&& f[len - 4] === '.'
			&& f[len - 3] === 'm'
			&& f[len - 2] === 'x'
			&& f[len - 1] === 'l');
}

function get_mxl_archive_from_mxl_file(mxl_file)
{
	const mxl_archive = mxl2irp.mxl_archive_create();
	if (!mxl_archive)
		return null;
	const unzipped = fflate.unzipSync(mxl_file);
	for (const file_name in unzipped) {
		const wasmFile = new mxl2irp.WasmString(unzipped[file_name]);
		const wasmFilename = new mxl2irp.WasmString(file_name);
		mxl2irp.mxl_archive_append_file(mxl_archive, wasmFilename.buf, wasmFile.buf, wasmFile.len);
	}
	return mxl_archive;
}

function get_song_from_path(path)
{
	let irp_song;
	try {
		if (is_mxl_file(path)) {
			const mxl_file = new Uint8Array(readFileSync(path));
			const mxl_archive = get_mxl_archive_from_mxl_file(mxl_file); 
			if (!mxl_archive)
				return null;
			const musicxml_file_index = mxl2irp.mxl_archive_get_musicxml_index(mxl_archive);
			if (musicxml_file_index < 0) {
				mxl2irp.mxl_archive_free(mxl_archive);
				return null;
			}
			const musicxml_buf = mxl2irp.mxl_archive_get_file_buf(mxl_archive, musicxml_file_index);
			const musicxml_len = mxl2irp.mxl_archive_get_file_len(mxl_archive, musicxml_file_index);
			irp_song = mxl2irp.parse_musicxml(musicxml_buf, musicxml_len);
			mxl2irp.mxl_archive_free(mxl_archive);
		} else {
			const musicxml_file = new mxl2irp.WasmString(readFileSync(path));
			irp_song = mxl2irp.parse_musicxml(musicxml_file.buf, musicxml_file.len);
			mxl2irp.free(musicxml_file.buf);
		}
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
