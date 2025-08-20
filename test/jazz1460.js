import {writeFileSync, readFileSync} from 'fs';
import {
  convertSync,
  convert,
  Playlist,
  Converter
} from 'ireal-musicxml'


function convert_1460() {
	const ireal = readFileSync('./test/irealpro/jazz1460.txt');
	const irealpro_playlist = new Playlist(ireal);

	for (const [i, song] of irealpro_playlist.songs.entries()) {
		try {
			const path = ('./test/musicxml/jazz1460/' + i.toString() + '_' + song.title + '.musicxml').replace(/ /g, '_');
			const musicxml = Converter.convert(song);
			writeFileSync(path, musicxml, 'utf8');
			console.log('Success : ' + path);
		} catch (err) {
			console.log(err);
			break;
		}
	}
}

convert_1460()
