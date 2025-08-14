import {
	initWasm,
	WasmString,
	parse_musicxml_song,
	irp_get_song_html,
	irp_song_free,
	free,
} from '/js/mxl2irp.js'
import wasmUrl from '/bin/libmxl2irp.wasm?url'

(async () =>{
	await initWasm(wasmUrl);
	window.target = document.getElementById('target');
	window.input = document.getElementById('file-input');
	input.addEventListener('change', (event) => {
		const file = event.target.files[0];
		target.textContent = '';
		if (!file) return ;
		let file_content;
		const reader = new FileReader();
		reader.onload = () => {
			file_content = reader.result;
			try {
				const file_wasm = new WasmString(file_content);
				const irp_song = parse_musicxml_song(file_wasm.buf, file_wasm.len);
				free(file_wasm.buf);
				if (!irp_song) return;
				target.innerHTML = irp_get_song_html(irp_song);
			} catch (err) {
				console.error(err);
			}
		}
		reader.readAsText(file);
	});
})();
