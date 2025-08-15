import Module from '../dist/libmxl2irp.js';

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
    constructor(js_str)
	{
		let encoded_buf = (new TextEncoder()).encode(js_str);
        this.len = encoded_buf.length;
        this.buf = malloc(this.len + 1);
        for (let i = 0; i < this.len; i++) {
            Wasm.HEAPU8[this.buf + i] = encoded_buf[i];
        }
        Wasm.HEAPU8[this.buf + this.len] = 0;
    }
	free()
	{
		free(this.buf);
	}
}

export async function initWasm(wasmPath) {
	const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

	let m;
	if (isNode) {
        m = await import('../dist/libmxl2irp.js').then(mod => mod.default());
	} else {
		m = await Module({ locateFile: f => f.endsWith('.wasm') ? wasmPath : f});
	}

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
