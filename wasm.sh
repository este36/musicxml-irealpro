#!/bin/bash

docker run --rm -v $(pwd):/src emscripten/emsdk bash -c "make wasm"
