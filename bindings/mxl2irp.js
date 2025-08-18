import Module from 'musicxml-irealpro/lib';
import { unzipSync } from 'fflate'

let mxl_archive_create = null;
let mxl_archive_free = null;
let mxl_archive_append_file = null;
let mxl_archive_get_musicxml_index = null;
let mxl_archive_get_file_buf = null;
let mxl_archive_get_file_len = null;
let mxl_archive_get_files_count = null;
let parse_musicxml = null;
let irp_song_get_html = null;
let irp_playlist_get_html = null;
let irp_playlist_create = null;
let irp_playlist_append = null;
let irp_playlist_free = null;
let irp_song_free = null;
let free = null;
let malloc = null;
let Wasm = null;

export class WasmString
{
    constructor(str)
	{
		let encoded_buf;
		if (typeof str === 'string') 
			encoded_buf = (new TextEncoder()).encode(str);
		else if (str instanceof Uint8Array)
			encoded_buf = str;
		else
			throw new Error("WasmString: cannot convert input");
        this.len = encoded_buf.length;
        this.buf = malloc(this.len + 1);
        for (let i = 0; i < this.len; i++) {
            Wasm.HEAPU8[this.buf + i] = encoded_buf[i];
        }
        Wasm.HEAPU8[this.buf + this.len] = 0;
    }
}

export function get_mxl_archive_from_mxl_file(mxl_file)
{
	const mxl_archive = mxl_archive_create();
	if (!mxl_archive)
		return null;
	const unzipped = unzipSync(mxl_file);
	for (const file_name in unzipped) {
		const wasmFile = new WasmString(unzipped[file_name]);
		const wasmFilename = new WasmString(file_name);
		mxl_archive_append_file(mxl_archive, wasmFilename.buf, wasmFile.buf, wasmFile.len);
	}
	return mxl_archive;
}

export function getIRealProSong(file_content, file_name)
{
	let irp_song;
	try {
		if (file_name.endsWith('.mxl')) {
			const mxl_file = new Uint8Array(file_content);
			const mxl_archive = get_mxl_archive_from_mxl_file(mxl_file); 
			if (!mxl_archive)
				return null;
			const musicxml_file_index = mxl_archive_get_musicxml_index(mxl_archive);
			if (musicxml_file_index < 0) {
				mxl_archive_free(mxl_archive);
				return null;
			}
			const musicxml_buf = mxl_archive_get_file_buf(mxl_archive, musicxml_file_index);
			const musicxml_len = mxl_archive_get_file_len(mxl_archive, musicxml_file_index);
			irp_song = parse_musicxml(musicxml_buf, musicxml_len);
			mxl_archive_free(mxl_archive);
		} else {
			const musicxml_file = new WasmString(file_content);
			irp_song = parse_musicxml(musicxml_file.buf, musicxml_file.len);
			free(musicxml_file.buf);
		}
		return irp_song != 0 ? irp_song : null;
	} catch (err) {
		console.error(err);
		return null;
	}
}

function readFileAsText(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = () => resolve(reader.result);
    reader.onerror = reject;
    reader.readAsText(file);
  });
}

export async function getIRealProSongFromHTMLInputFile(input_file)
{
	if (!input_file) return null;
	const file_content = await readFileAsText(input_file);
	if (!file_content)
		return null;
	return getIRealProSong(file_content, input_file.name);
}

export async function initWasm(wasmPath) {
	const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

	let m;
	if (isNode) {
        m = await import('musicxml-irealpro/lib').then(mod => mod.default());
	} else {
		m = await Module({ locateFile: f => f.endsWith('.wasm') ? wasmPath : f});
	}

	mxl_archive_create = m.cwrap('mxl_archive_create', 'number', null);
	mxl_archive_free = m.cwrap('mxl_archive_free', null, ['number']);
	mxl_archive_append_file = m.cwrap('mxl_archive_append_file', null, ['number', 'number','number','number']);
	mxl_archive_get_musicxml_index = m.cwrap('mxl_archive_get_musicxml_index', 'number', ['number']);
	mxl_archive_get_file_buf = m.cwrap('mxl_archive_get_file_buf', 'number', ['number', 'number']);
	mxl_archive_get_file_len = m.cwrap('mxl_archive_get_file_len', 'number', ['number', 'number']);
	mxl_archive_get_files_count = m.cwrap('mxl_archive_get_files_count', 'number', ['number']);
    parse_musicxml = m.cwrap('parse_musicxml', 'number', ['number', 'number']);
    irp_song_get_html = m.cwrap('irp_song_get_html', 'string', ['number']);
    irp_playlist_get_html = m.cwrap('irp_playlist_get_html', 'string', ['number']);
	irp_playlist_create = m.cwrap('irp_playlist_create', 'number', ['string']);
	irp_playlist_append = m.cwrap('irp_playlist_append', null, ['number', 'number']);
	irp_playlist_free = m.cwrap('irp_playlist_free', null, ['number']);
    irp_song_free = m.cwrap('irp_song_free', null, ['number']);
    free = m.cwrap('free', null, ['number']);
    malloc = m.cwrap('malloc', 'number', ['number']);
	Wasm = m;
}

export {
	mxl_archive_create,
	mxl_archive_free,
	mxl_archive_append_file,
	mxl_archive_get_musicxml_index,
	mxl_archive_get_file_buf,
	mxl_archive_get_file_len,
	mxl_archive_get_files_count,
	parse_musicxml,
	irp_song_get_html,
	irp_song_free,
	irp_playlist_create,
	irp_playlist_append,
	irp_playlist_get_html,
	irp_playlist_free,
	free,
	malloc,
	Wasm,
}
