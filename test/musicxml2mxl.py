import glob
from test.do_test import exec_cmd

jazz1460 = glob.glob("./test/musicxml/jazz1460/*.musicxml")
for index, f in enumerate(jazz1460):
	print(str(index) + '/1460');
	exec_cmd(['/home/esteban/.local/bin/musescore4portable', f, '-o', f.replace('musicxml', 'mxl').replace('musicxml', 'mxl')])
