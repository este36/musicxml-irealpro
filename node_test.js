import fs from "fs/promises";
import path from "path";
import Module from "./build-wasm/mxl_to_irealpro.js";

const songStructSize = 568; // adapter taille structure réelle

function allocString(Module, str) {
  const encoder = new TextEncoder();
  const encoded = encoder.encode(str);
  const ptr = Module._malloc(encoded.length + 1);
  Module.HEAPU8.set(encoded, ptr);
  Module.HEAPU8[ptr + encoded.length] = 0;
  return { ptr, length: encoded.length };
}

function readCString(Module, ptr) {
  const heap = Module.HEAPU8;
  let end = ptr;
  while (heap[end] !== 0) end++;
  const slice = heap.subarray(ptr, end);
  return new TextDecoder().decode(slice);
}

async function loadFile(filepath) {
  return fs.readFile(filepath, "utf8");
}

async function allocFile(Module, filepath) {
  const content = await loadFile(filepath);
  return allocString(Module, content);
}

Module.onRuntimeInitialized = async () => {
  const songsPtr = Module._malloc(songStructSize * 2);
  Module.HEAPU8.fill(0, songsPtr, songsPtr + songStructSize * 2);

  {
    const { ptr: filePtr, length: fileLen } = await allocFile(Module, "musicxml/Misty.musicxml");
    const ret = Module._parse_musicxml_song(songsPtr, filePtr, fileLen);
    if (ret !== 0) {
      console.error("PARSER_STOP_ERROR for Misty.musicxml");
      Module._free(filePtr);
      return;
    }
    Module._free(filePtr);
  }

  {
    const { ptr: filePtr, length: fileLen } = await allocFile(Module, "musicxml/Out_of_Nothing.musicxml");
    const ret = Module._parse_musicxml_song(songsPtr + songStructSize, filePtr, fileLen);
    if (ret !== 0) {
      console.error("PARSER_STOP_ERROR for Out_of_Nothing.musicxml");
      Module._free(filePtr);
      return;
    }
    Module._free(filePtr);
  }

  const { ptr: playlist_name } = allocString(Module, "Wassup");
  const htmlPtr = Module._irp_get_playlist_html(playlist_name, songsPtr, 2);
  const html = readCString(Module, htmlPtr);

  console.log(html);

  Module._irp_song_free(songsPtr);
  Module._irp_song_free(songsPtr + songStructSize);
  Module._free(htmlPtr);
  Module._free(songsPtr);
};
