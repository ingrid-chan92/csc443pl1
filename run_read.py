from subprocess import Popen, PIPE
import sys
import re

KB = 1024
MB = 1024 * 1024
filename = 'randfile.out'
filesize = 50 * MB 
sizes = (
    10,
    100,
    1 * KB,
    5 * KB,
    11 * KB,
    101 * KB,
    301 * KB,
    501 * KB,
    1 * MB,
    2 * MB,
    3 * MB
)

perf = []
cmd = 'get_histogram' if not sys.argv[1:] else 'random_read'
for sz in sizes:
    p = Popen(
        ['./%s' % cmd, filename, '%d' % sz],
        stdout=PIPE)
    s = p.stdout.read().strip()
    print '>%s' % s
    perf.append(map(int, re.findall(r'\d+', s)))

import matplotlib
from pylab import *
figure(1)
x = [d[1] for d in perf]
y = [d[2] for d in perf]
loglog(x, y)
xlabel('block size')
ylabel('milliseconds')
title(cmd)
savefig('%s.png' % cmd)
with open('%s.csv' % cmd, 'w') as out:
    print >>out, "\n".join("%s,%s" % (x[1],x[2]) for x in perf)
