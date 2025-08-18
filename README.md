# musicxml-irealpro
### Musicxml to iReal Pro File Converter.
Provides a library written in C, wasm bindings for both nodejs and the browser.

Both compressed `.mxl` files and `.musicxml` are supported.

Irealpro does not support this feature natively.
This makes easier to import directly your scores to IrealPro especially if you are a composer.

# Installation

#### Dependencies :
* [gcc](https://gcc.gnu.org/)
* [make](https://en.wikipedia.org/wiki/Make_(software))
* [docker](https://www.docker.com/)
* [node](https://nodejs.org/en)
* [npm](https://www.npmjs.com/)

The Makefile provides three main rules: `lib_a` (static lib), `lib_so` (shared lib), and `lib_js` for the wasm target.

By default, every libraries are built.
```bash
git clone https://github.com/este36/musicxml-irealpro.git --depth=1
cd musicxml-irealpro
make -B
```
# Usage

inside the `./test/` folder there is two utilities, `test.c` and `test.js`. They both provides a cli tool wich print the resulted irealpro file to standart output.
For now its the easier way to understand the apis. I am planning to write a proper documentation.

To build the clis :
```bash
make test/test.out
npm install
```
CLI Usage :
```bash
./test/test.out (one or more)[path/to/file[.musicxml|.mxl]] 
```
or 
```bash
./test/test.js (one or more)[path/to/file[.musicxml|.mxl]]
```
# References

* https://www.w3.org/2021/06/musicxml40/musicxml-reference/element-tree/
* https://www.irealpro.com/ireal-pro-custom-chord-chart-protocol
* https://github.com/infojunkie/ireal-musicxml
