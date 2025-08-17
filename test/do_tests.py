import subprocess
import glob
import sys

def exec_cmd(args):
	print(' '.join(args))
	return subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

def js_cmd(args):
    args = args[:]
    args.insert(0, "./test/test.js")
    args.insert(0, "node")
    return exec_cmd(args)

def c_cmd(args):
    args = args[:]
    args.insert(0, "./test/test.out")
    return exec_cmd(args)

def do_test(args, should_fail):
	c_res = c_cmd(args)
	js_res = js_cmd(args)
	if should_fail:
		res = c_res.stderr != "" and js_res.stderr != ""
	else:
		res = c_res.stderr == "" and js_res.stderr == "" and c_res.stdout == js_res.stdout
	print("\033[32mOK\033[0m" if res else "\033[31mKO\033[0m")
	if not res:
		print("C result:\n")
		print("stdout:")
		print(repr(c_res.stdout))
		print("stderr:")
		print(repr(c_res.stderr))
		print("JS result:\n")
		print("stdout:")
		print(repr(js_res.stdout))
		print("stderr:")
		print(repr(js_res.stderr))
		sys.exit(1)

musicxml_files = glob.glob("./test/musicxml/*.musicxml")

for f in musicxml_files:
    do_test([f], False)

do_test(musicxml_files, False)

mxl_files = [
	"./test/mxl/Out_of_Nothing.mxl",
	"./test/mxl/complicité.mxl",
	"./test/mxl/timesignature.mxl"
]

do_test([mxl_files[0]], False)
do_test([mxl_files[1]], False)
do_test([mxl_files[2]], True)
do_test(mxl_files, True)
mxl_files.pop()
do_test(mxl_files, True)
