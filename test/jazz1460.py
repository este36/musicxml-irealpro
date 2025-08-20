#!/usr/bin/env python3
import glob
from do_test import do_test 

if __name__ == "__main__":
    jazz1460 = glob.glob("./test/mxl/jazz1460/*.mxl")
    for index, f in enumerate(jazz1460):
        print(f"Progress: {index}/{len(jazz1460)}")
        do_test([f], False)
