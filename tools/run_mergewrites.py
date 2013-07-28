#!/usr/bin/env python
# This is a Python script for running a program on Marmot and extracting
# performance results to a text file.
# 
# Things need to do in this script:
# 1. clean cache/buffer before and after each run
# 2. collect performance data and format it nicely
# 3. able to run several test programs in a batch (or maybe not)
#
# Let me build a prototype by using mergewrites
import itertools,subprocess
import sys
from time import gmtime, strftime

exefile = sys.argv[1] # path of exe file
mappath = "../../tools/"
mapfile = ["flash64.map"]
mapfile = [mappath+s for s in mapfile]
fetch_buf_size = ["100", "500"]
domerge = ["0", "1"]
dosort = ["0", "1"]
doflush = ["0", "1"]
flushdir = ["./flushdir"]

parameters = [mapfile, fetch_buf_size, domerge, dosort, doflush, flushdir]
paralist = list(itertools.product(*parameters))

jobid = strftime("%Y-%m-%d-%H-%M-%S", gmtime())
logname = jobid + ".log"
resultname = jobid + ".result"


if __name__ == '__main__':
    logf = open(logname, 'a')
    resultf = open(resultname, 'a')
    
    for para in paralist:
        cmd = [exefile] + list(para)
        print cmd
        proc = subprocess.Popen(cmd,
                stdout=subprocess.PIPE,
                stderr=logf)
        proc.wait()

        # result parsing
        for line in proc.stdout:
            print line,
            resultf.write(line)
            sys.stdout.flush()

    logf.close()
    resultf.close()


