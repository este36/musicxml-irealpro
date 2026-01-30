# musicxml-irealpro

Try the web app : [mxltoireal.com](https://www.mxltoireal.com)

### MusicXML to iReal Pro File Converter
Provides a library written in C, with WebAssembly bindings for both Node.js and the browser.

Both compressed `.mxl` files and `.musicxml` files are supported.

iReal Pro does not support this feature natively.  
This makes it easier to import your scores directly into iReal Pro, especially if you are a composer.

---

# Installation

```bash
npm install musicxml-irealpro
```
### Dependencies
* [gcc](https://gcc.gnu.org/)
* [make](https://en.wikipedia.org/wiki/Make_(software))
#### Only to build the javascript library :
* [docker](https://www.docker.com/)
* [node](https://nodejs.org/en)
* [npm](https://www.npmjs.com/)

#### Makefile main rules :  
- `lib_a` → static library  
- `lib_so` → shared library  
- `lib_js` → WebAssembly target  
- `mxl2irp` → cli tool

By default, all libraries are built.

# Usage

inside the `./cli/` folder there are two utilities : `main.c` and `index.js`. 

They both provide a CLI tool that prints the resulting iReal Pro file to standard output.

For now, this is the easiest way to understand the APIs. I am planning to write a proper documentation.

### Build the CLIs
```bash
npm install
make mxl2irp
```
### CLI Usage :
```bash
./mxl2irp (one or more)[path/to/file[.musicxml|.mxl]] 
```
or 
```bash
./cli/index.js (one or more)[path/to/file[.musicxml|.mxl]]
```

# References

* https://www.w3.org/2021/06/musicxml40/musicxml-reference/element-tree/
* https://www.irealpro.com/ireal-pro-custom-chord-chart-protocol
* https://github.com/infojunkie/ireal-musicxml
* https://effendi.me/jazz/
