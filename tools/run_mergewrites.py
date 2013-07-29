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
import os
from time import gmtime, strftime

exefile = sys.argv[1] # path of mergewrites
mappath = "/home/junhe/workdir/mapfiles/"
mapfile = ["flash64.map"]
#mapfiles = ['LANL_App1.dmp0.map', 'btio.16pe.simple.map', 'flash.64PE.hdf5_plt_crn_0000.map', 'app2_ind.map', 'app3.map', 'fstest.map', 'app2_iolib.map', 'app2_mpiio_col.map', 'flash.64PE.hdf5_plt_cnt_0000.map', 'flash64.map', 'flash.64PE.hdf5_chk_0000.map', 'chombo64.map']
#mapfile = [mappath+s for s in mapfile]
fetch_buf_size = ["100", "500"]
domerge = ["0", "1"]
dosort = ["0", "1"]
doflush = ["0", "1"]
flushprefix = "/tmp/"
# use the mapfile name to be the dir instead of this
#flushdir = ["./flushdir"] 


parameters = [mapfile, fetch_buf_size, domerge, dosort, doflush]
paralist = list(itertools.product(*parameters))

jobid = strftime("%Y-%m-%d-%H-%M-%S", gmtime())
logname = jobid + ".log"
resultname = jobid + ".result"


if __name__ == '__main__':
    logf = open(logname, 'a')
    resultf = open(resultname, 'a')
    
    for para in paralist:
        para = list(para)
        flushdirpath = flushprefix + para[0]
        para.append(flushdirpath) # flush dir path
        para[0] = mappath + para[0] # make full mapfile path here
        cmd = [exefile] + para
    
        if not os.path.exists(flushdirpath):
            os.makedirs(flushdirpath)


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


