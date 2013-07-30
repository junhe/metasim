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
import shlex

if len(sys.argv) != 2:
    print "wrong arguments"
    sys.exit(0)

exefile = sys.argv[1] # path of readindex_driver
indexdir = "/tmp/"
#index_vfile = ["flash64.map"]
#index_vfile = ["chombo64.map"]
index_vfile = ['LANL_App1.dmp0.map', 'btio.16pe.simple.map', 'flash.64PE.hdf5_plt_crn_0000.map', 'app2_ind.map', 'app3.map', 'fstest.map', 'app2_iolib.map', 'app2_mpiio_col.map', 'flash.64PE.hdf5_plt_cnt_0000.map', 'flash64.map', 'flash.64PE.hdf5_chk_0000.map', 'chombo64.map']
#mapfile = [mappath+s for s in mapfile]

parameters = [index_vfile]
paralist = list(itertools.product(*parameters))

jobid = strftime("%Y-%m-%d-%H-%M-%S", gmtime())
logname = jobid + ".log"
resultname = jobid + ".result"

def drop_local_cache ():
    proc = subprocess.Popen(
            shlex.split("sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'"))
    proc.wait()

    proc = subprocess.Popen(
            shlex.split("sudo sh -c 'sync'"))
    proc.wait()


if __name__ == '__main__':
    logf = open(logname, 'a')
    resultf = open(resultname, 'a')
   
    for i in range(0, 5):
        for para in paralist:
            para = list(para)
            para[0] = indexdir + para[0] # prefix with dirpath
            cmd = [exefile] + para
        
            print cmd
            drop_local_cache()

            proc = subprocess.Popen(cmd,
                    stdout=subprocess.PIPE,
                    stderr=logf)
            proc.wait()

            drop_local_cache()

            # result parsing
            for line in proc.stdout:
                print line,
                resultf.write(line)
                sys.stdout.flush()

    logf.close()
    resultf.close()


