import difflib
import subprocess
import glob
import sys
from do_test import do_test

musicxml_files = glob.glob("./test/musicxml/*.musicxml")

unvalids = ["./test/musicxml/strange.musicxml"]

for i, f in enumerate(musicxml_files):
	should_fail = (f in unvalids)
	do_test([f], should_fail)
	if should_fail:
		musicxml_files.pop(i)

do_test(musicxml_files, False)

exit(0)

mxl_files = [
	"./test/mxl/Out_of_Nothing.mxl",
	"./test/mxl/complicité.mxl",
	"./test/mxl/multipart.mxl",
	"./test/mxl/timesignature.mxl"
]

do_test([mxl_files[0]], False)
do_test([mxl_files[1]], False)
do_test([mxl_files[2]], False)
do_test([mxl_files[3]], True)

do_test(mxl_files, True)
mxl_files.pop()
do_test(mxl_files, False)

all_files = mxl_files + musicxml_files
do_test(all_files, False)


jazz1460 = glob.glob("./test/musicxml/jazz1460/*.musicxml")

for f in jazz1460:
    do_test([f], False)
