import {writeFileSync, readFileSync} from 'fs';
import {
  convertSync,
  convert,
  Playlist,
  Converter
} from 'ireal-musicxml'


function irealmxl(irpfile) {
	const ireal = readFileSync('./test/irealpro/' + irpfile + '.txt');
	const irealpro_playlist = new Playlist(ireal);

	for (const [i, song] of irealpro_playlist.songs.entries()) {
		try {
			const path = ('./test/musicxml/' + irpfile + i.toString() + '_' + song.title + '.musicxml').replace(/ /g, '_');
			const musicxml = Converter.convert(song);
			writeFileSync(path, musicxml, 'utf8');
			console.log('Success : ' + path);
		} catch (err) {
			console.log(err);
			break;
		}
	}
}

// irealmxl('pop400')
