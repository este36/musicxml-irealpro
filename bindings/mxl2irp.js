
import { unzipSync } from 'fflate'

export let get_error_code_str = null;
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
export let irp_playlist_get_html = null;
export let irp_playlist_create = null;
export let irp_playlist_append = null;
export let irp_playlist_free = null;
export let irp_song_free = null;
export let free = null;
export let malloc = null;
export let Wasm = null;

export async function initWasm(wasmPath)
{
	const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

	let m;
	if (isNode) m = await import('musicxml-irealpro/lib').then(mod => mod.default());
	else m = await Module({ locateFile: f => f.endsWith('.wasm') ? wasmPath : f});

	get_error_code_str = m.cwrap('get_error_code_str', 'string', ['number']);
	mxl2irp_result_get_error_code = m.cwrap('mxl2irp_result_get_error_code', 'number', ['number']);
	mxl2irp_result_get_item =  m.cwrap('mxl2irp_result_get_item', 'number', ['number']);
	mxl2irp_result_get_error_details = m.cwrap('mxl2irp_result_get_error_details', 'string', ['number']);
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
	const unzipped = unzipSync(mxl_file);
	for (const file_name in unzipped) {
		const wasmFile = new WasmString(unzipped[file_name]);
		const wasmFilename = new WasmString(file_name);
		mxl_archive_append_file(mxl_archive, wasmFilename.buf, wasmFile.buf, wasmFile.len);
	}
	return mxl_archive;
}

function getFileContent(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = () => resolve(reader.result);
    reader.onerror = reject;
    reader.readAsArrayBuffer(file);
  });
}

export async function getIRealProSong(file_content, file_name)
{
	if (file_content instanceof File) {
		file_content = await getFileContent(file_content);
	}
	let result;
	if (file_name.endsWith('.mxl')) {
		const mxl_archive = get_mxl_archive_from_mxl_file(file_content); 
		if (!mxl_archive)
			return null;
		const musicxml_file_index = mxl_archive_get_musicxml_index(mxl_archive);
		if (musicxml_file_index < 0) {
			mxl_archive_free(mxl_archive);
			return null;
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
		return null;
	result = {
		error_code: mxl2irp_result_get_error_code(result),
		error_details: mxl2irp_result_get_error_details(result),
		item: mxl2irp_result_get_item(result),
	};
	if (result.error_code != 0) {
		const msg = "Error: " + file_name + ': ' + get_error_code_str(result.error_code)
				  + (result.error_details ? "\nDetails: " + result.error_details : '');
		console.error(msg);
		free(result.error_details);
		return null;
	}
	return result.item;
}
