# musicxml-irealpro
### MusicXML to iReal Pro File Converter
Provides a library written in C, with WebAssembly bindings for both Node.js and the browser.

Both compressed `.mxl` files and `.musicxml` files are supported.

iReal Pro does not support this feature natively.  
This makes it easier to import your scores directly into iReal Pro, especially if you are a composer.

---

## Installation
```bash
npm install musicxml-irealpro
```
### Dependencies
* [gcc](https://gcc.gnu.org/)
* [make](https://en.wikipedia.org/wiki/Make_(software))
##### For building the javascript library:
* [docker](https://www.docker.com/)
* [node](https://nodejs.org/en)
* [npm](https://www.npmjs.com/)

The `Makefile` provides three main rules :  
- `lib_a` → static library  
- `lib_so` → shared library  
- `lib_js` → WebAssembly target  

By default, all libraries are built.

```bash
git clone https://github.com/este36/musicxml-irealpro.git --depth=1
cd musicxml-irealpro
make -B
```
# Usage

inside the `./test/` folder there are two utilities : `test.c` and `test.js`. 

They both provide a CLI tool that prints the resulting iReal Pro file to standard output.

For now, this is the easiest way to understand the APIs. I am planning to write a proper documentation.

Build the CLIs
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
* https://effendi.me/jazz/
