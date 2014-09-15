from subprocess import Popen, PIPE
import sys
import re

KB = 1024
MB = 1024 * 1024
filename = 'randfile.out'
filesize = 50 * MB 
sizes = (
    100,
    1 * KB,
    5 * KB,
    10 * KB,
    100 * KB,
    300 * KB,
    500 * KB,
    1 * MB,
    2 * MB,
    3 * MB
)

perf = []
for sz in sizes:
    p = Popen(
        ['./create_random_file', filename, '%d' % filesize, '%d' % sz],
        stdout=PIPE)
    s = p.stdout.read().strip()
    print '>%s' % s
    perf.append(map(int, re.findall(r'\d+', s)))

import matplotlib
from pylab import *
figure(1)
x = [d[0] for d in perf]
y = [d[1] for d in perf]
loglog(x, y)
xlabel('block size')
ylabel('milliseconds')
savefig('write.png')
