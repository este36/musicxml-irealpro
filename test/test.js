#!/usr/bin/env node

import { readFileSync } from 'fs';
import * as mxl2irp from 'musicxml-irealpro';

function get_song_from_path(path)
{
	return mxl2irp.getIRealProSong(readFileSync(path), path);
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
		const result = get_song_from_path(argv[1]);
		if (result.error_code !== 0) {
			return 1;
		}
		const url = mxl2irp.irp_song_get_html(result.item);
		if (url === null) {
			console.error(`HTML RENDER FAIL`);
			return 1;
		}
		print_url(url);
		mxl2irp.irp_song_free(result.item);
		mxl2irp.free(url);
	} else {
		let irp_playlist = mxl2irp.irp_playlist_create("Test");
		for (let i = 1; i < argc; i++) {
			const result = get_song_from_path(argv[i]);
			if (result.error_code !== 0) {
				mxl2irp.irp_playlist_free(irp_playlist);
				return 1;
			}
			mxl2irp.irp_playlist_append(irp_playlist, result.item);
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
