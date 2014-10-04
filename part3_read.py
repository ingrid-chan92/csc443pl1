#!/usr/bin/env python

import os
import re
import subprocess
import sys
import matplotlib.pyplot as plt
from time import sleep

data_dir = "../data/context_switch"

def get_next_id():
    fnames = filter(lambda x: x.startswith("threshold_fig") and x.endswith(".png"), os.listdir(data_dir))
    n = len("threshold_fig")
    nums = []
    for f in fnames:
        i = n
        while f[i].isdigit():
            i += 1
        nums.append(int(f[n:i]))

    return max(nums) + 1

def cmd(command):
    '''Runs the given shell command and returns its standard output as a string.
    Throws an exception if the command returns anything other than 0.'''
    #p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #print "waiting for process with PID %d" % p.pid
    #ret = os.waitpid(p.pid, 0)[1]
    #if ret != 0:
        #print p.stdout.read()
        #raise Exception('Command [%s] failed with status %d\n:%s' % (command, ret, p.stderr.read()))
    #return p.stdout.read()
    return subprocess.check_output(command, universal_newlines=True)


if __name__ == "__main__":

    # Build the program.
    cmd(['make'])

    # Run the program and create the plot.
    #output = cmd(['./time_inactive', str(num)])
    graph_value = [];

    for pgsize in range(2,33):
        output = cmd(['./read_fixed_len_page', 'results'+str(pgsize*1000) , str(pgsize*1000)])
        time = output.splitlines()
        millisec = time[-1].split()
        graph_value.append(float(millisec[1]))
        sleep(5.5)
    
    #+print graph_value 
   
    x =range(2000,33000,1000)

    #make sure that the threshold line draw *after* the intervals
    plt.plot(x, graph_value, 'bd-')
    #print "saving graph to %s/threshold_fig%d_n_%d.png" % (data_dir, get_next_id(), num)
    plt.savefig("experiment 3 read graph")
    plt.show()
