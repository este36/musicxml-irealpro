import { unzipSync } from 'fflate'
import Module from 'musicxml-irealpro/lib'

export const STYLE_DEFAULT = 15;

export let get_error_code_str = null;
export let get_style_str = null;
export let get_style_default_tempo = null;
export let get_note_str = null;
export let mxl2irp_result_get_error_code = null;
export let mxl2irp_result_get_item = null;
export let mxl2irp_result_get_error_details = null;
export let mxl_archive_create = null;
export let mxl_archive_free = null;
export let mxl_archive_append_file = null;
export let mxl_archive_get_musicxml_index = null;
export let mxl_archive_get_file_buf = null;
export let mxl_archive_get_file_len = null;
export let mxl_archive_get_files_count = null;
export let parse_musicxml = null;
export let irp_song_get_html = null;
export let irp_song_get_title = null;
export let irp_song_set_title = null;
export let irp_song_get_composer = null;
export let irp_song_set_composer = null;
export let irp_song_get_key = null;
export let irp_song_get_tempo = null;
export let irp_song_set_tempo = null;
export let irp_song_get_style = null;
export let irp_song_set_style = null;
export let irp_playlist_get_html = null;
export let irp_playlist_create = null;
export let irp_playlist_append = null;
export let irp_playlist_free = null;
export let irp_song_free = null;
export let free = null;
export let malloc = null;
export let Wasm = null;

export const ErrorEnum = {
	ERROR_UNSET: 1,
	ERROR_XML_FILE_CORRUPT: 2,
	ERROR_EMPTY_SCORE: 3,
	ERROR_ZOOM_FAILED: 3,
	ERROR_TOO_MUCH_CHORDS: 4,
	ERROR_TOO_MUCH_DEGREES: 5,
	ERROR_UNVALID_CHORD_KIND: 6,
	ERROR_UNVALID_KEY: 7,
	ERROR_UNVALID_DIVISIONS: 8,
	ERROR_UNVALID_TIME_SIGNATURE: 9,
	ERROR_CREDENTIALS_OVERFLOW: 10,
	ERROR_COUNT: 11
};

export async function initWasm(wasmPath)
{
	const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

	let m;
	if (isNode) m = await import('musicxml-irealpro/lib').then(mod => mod.default());
	else m = await Module({ locateFile: f => f.endsWith('.wasm') ? wasmPath : f});

	get_error_code_str = m.cwrap('get_error_code_str', 'string', ['number']);
	get_style_str = m.cwrap('get_style_str', 'string', ['number']);
	get_style_default_tempo = m.cwrap('get_style_default_tempo', 'number', ['number']);
	get_note_str = m.cwrap('get_note_str', 'string', ['number']);
	mxl2irp_result_get_error_code = m.cwrap('mxl2irp_result_get_error_code', 'number', ['number']);
	mxl2irp_result_get_item =  m.cwrap('mxl2irp_result_get_item', 'number', ['number']);
	mxl2irp_result_get_error_details = m.cwrap('mxl2irp_result_get_error_details', 'number', ['number']);
	mxl_archive_create = m.cwrap('mxl_archive_create', 'number', null);
	mxl_archive_free = m.cwrap('mxl_archive_free', null, ['number']);
	mxl_archive_append_file = m.cwrap('mxl_archive_append_file', null, ['number', 'number','number','number']);
	mxl_archive_get_musicxml_index = m.cwrap('mxl_archive_get_musicxml_index', 'number', ['number']);
	mxl_archive_get_file_buf = m.cwrap('mxl_archive_get_file_buf', 'number', ['number', 'number']);
	mxl_archive_get_file_len = m.cwrap('mxl_archive_get_file_len', 'number', ['number', 'number']);
	mxl_archive_get_files_count = m.cwrap('mxl_archive_get_files_count', 'number', ['number']);
    parse_musicxml = m.cwrap('parse_musicxml', 'number', ['number', 'number']);
    irp_song_get_html = m.cwrap('irp_song_get_html', 'number', ['number']);
	irp_song_get_title = m.cwrap('irp_song_get_title', 'string', ['number']);
	irp_song_set_title = m.cwrap('irp_song_set_title', null, ['number', 'string']);
	irp_song_get_composer = m.cwrap('irp_song_get_composer', 'string', ['number']);
	irp_song_set_composer = m.cwrap('irp_song_set_composer', null, ['number', 'string']);
	irp_song_get_key = m.cwrap('irp_song_get_key', 'number', ['number']);
	irp_song_get_tempo = m.cwrap('irp_song_get_tempo', 'number', ['number']);
	irp_song_set_tempo = m.cwrap('irp_song_set_tempo', 'number', ['number', 'number']);
	irp_song_get_style = m.cwrap('irp_song_get_style', 'number', ['number']);
	irp_song_set_style = m.cwrap('irp_song_set_style', 'number', ['number', 'number']);
    irp_playlist_get_html = m.cwrap('irp_playlist_get_html', 'number', ['number']);
	irp_playlist_create = m.cwrap('irp_playlist_create', 'number', ['string']);
	irp_playlist_append = m.cwrap('irp_playlist_append', null, ['number', 'number']);
	irp_playlist_free = m.cwrap('irp_playlist_free', null, ['number']);
    irp_song_free = m.cwrap('irp_song_free', null, ['number']);
    free = m.cwrap('free', null, ['number']);
    malloc = m.cwrap('malloc', 'number', ['number']);
	Wasm = m;
}

class WasmString
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

export function get_mxl_archive_from_mxl_file(file_content)
{
	const mxl_file = new Uint8Array(file_content);
	const mxl_archive = mxl_archive_create();
	if (!mxl_archive)
		return null;
	let unzipped;
	try {
		unzipped = unzipSync(mxl_file);
	} catch (err) {
		return null;
	}
	for (const file_name in unzipped) {
		const wasmFile = new WasmString(unzipped[file_name]);
		const wasmFilename = new WasmString(file_name);
		mxl_archive_append_file(mxl_archive, wasmFilename.buf, wasmFile.buf, wasmFile.len);
	}
	return mxl_archive;
}

export function getIRealProSong(file_content, file_name)
{
	const error = {
		error_code: ErrorEnum.ERROR_XML_FILE_CORRUPT
	}
	let result = {};
	if (file_name.endsWith('.mxl')) {
		const mxl_archive = get_mxl_archive_from_mxl_file(file_content); 
		if (!mxl_archive)
			return error;
		const musicxml_file_index = mxl_archive_get_musicxml_index(mxl_archive);
		if (musicxml_file_index < 0) {
			mxl_archive_free(mxl_archive);
			return error;
		}
		const musicxml_buf = mxl_archive_get_file_buf(mxl_archive, musicxml_file_index);
		const musicxml_len = mxl_archive_get_file_len(mxl_archive, musicxml_file_index);
		result = parse_musicxml(musicxml_buf, musicxml_len);
		mxl_archive_free(mxl_archive);
	} else {
		const musicxml_file = new WasmString(file_content);
		result = parse_musicxml(musicxml_file.buf, musicxml_file.len);
		free(musicxml_file.buf);
	}
	if (!result)
		return error;
	result = {
		error_code: mxl2irp_result_get_error_code(result),
		error_details: mxl2irp_result_get_error_details(result),
		item: mxl2irp_result_get_item(result),
	};
	if (result.error_code != 0) {
		const msg = "Error: " + file_name + ': ' + get_error_code_str(result.error_code)
				  + (result.error_details ? "\nDetails: " + Wasm.UTF8ToString(result.error_details) : '');
		console.error(msg);
		if (result.error_details)
			free(result.error_details);
	}
	return result;
}
