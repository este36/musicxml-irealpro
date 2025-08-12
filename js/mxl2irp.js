import Module from '../bin/libmxl2irp.js';

export let parse_musicxml_song = null;
export let irp_get_song_html = null;
export let irp_get_playlist_html = null;
export let irp_playlist_create = null;
export let irp_playlist_append = null;
export let irp_playlist_free = null;
export let irp_song_free = null;
export let free = null;
export let malloc = null;
export let Wasm = null;

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

/**
 * Initialise le module WASM et expose mxl2irp.
 * Utilisation : await initWasm();
 * mxl2irp sera alors disponible.
 */
export async function initWasm() {
	const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

	let m;
	if (isNode) {
        m = await import('../bin/libmxl2irp.js').then(mod => mod.default());
	} else {
		m = window.Module || {};
	}

    parse_musicxml_song = m.cwrap('parse_musicxml_song', 'number', ['number', 'number']);
    irp_get_song_html = m.cwrap('irp_get_song_html', 'string', ['number']);
    irp_get_playlist_html = m.cwrap('irp_get_playlist_html', 'string', ['number']);
	irp_playlist_create = m.cwrap('irp_playlist_create', 'number', ['string']);
	irp_playlist_append = m.cwrap('irp_playlist_append', null, ['number', 'number']);
	irp_playlist_free = m.cwrap('irp_playlist_free', null, ['number']);
    irp_song_free = m.cwrap('irp_song_free', null, ['number']);
    free = m.cwrap('free', null, ['number']);
    malloc = m.cwrap('malloc', 'number', ['number']);
	Wasm = m;
}
