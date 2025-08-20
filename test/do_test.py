import difflib
import subprocess
import glob
import sys
import glob

def exec_cmd(args):
	print(' '.join(args))
	return subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

def js_cmd(args):
    args = args[:]
    args.insert(0, "./test/test.js")
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
	print(("\033[32mOK\033[0m" if res else "\033[31mKO\033[0m") + (" (Should fail)" if should_fail else ""))
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
		if js_res.stdout != c_res.stdout:
		    print("diff stdout:")
		    diff = difflib.unified_diff(
		        js_res.stdout.splitlines(),
		        c_res.stdout.splitlines(),
		        fromfile='js_stdout',
		        tofile='c_stdout',
		        lineterm=''
		    )
		    print('\n'.join(diff))
		if js_res.stderr != c_res.stderr:
		    print("diff stderr:")
		    diff = difflib.unified_diff(
		        js_res.stdout.splitlines(),
		        c_res.stdout.splitlines(),
		        fromfile='js_stderr',
		        tofile='c_stderr',
		        lineterm=''
		    )
		    print('\n'.join(diff))
#sys.exit(1)
