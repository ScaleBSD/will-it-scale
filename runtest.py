#!/usr/bin/env python

import time
import subprocess
import sys
import os
import re

class wis_stat():
	def __init__(self, procstat='/proc/stat'):
		fd = open(procstat, 'r');
		for line in fd.readlines():
			arr = line.split()
			if arr[0] != 'cpu':
				continue

			self.user = int(arr[1])
			self.nice = int(arr[2])
			self.system = int(arr[3])
			self.idle = int(arr[4])
			self.iowait = 0
			self.irq = 0
			self.softirq = 0
			self.steal = 0
			self.guest = 0
			self.guest_nice = 0
			if len(arr) > 8:
				self.steal = int(arr[8])
			if len(arr) > 9:
				self.guest = int(arr[9])
			if len(arr) > 10:
				self.guest_nice = int(arr[10])

			break
		fd.close()
	def idle_fraction(self, prev):
		busy = self.user + self.nice + self.system + self.irq + self.softirq + self.steal + self.guest + self.guest_nice
		idle = self.idle + self.iowait

		if prev:
			busy = busy - (prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal + prev.guest + prev.guest_nice)
			idle = idle - (prev.idle + prev.iowait)

		if (idle + busy) == 0:
			return 0

		return 1.0 * idle / (idle + busy)


duration=4

if len(sys.argv) != 2:
	print >> sys.stderr, 'Usage: runtest.py <testcase>'
	sys.exit(1)
cmd = sys.argv[1]

pipe = subprocess.Popen('uname -m', shell=True, stdout=subprocess.PIPE).stdout
arch = pipe.readline().rstrip(os.linesep)
pipe.close()
pipe = subprocess.Popen("uname -s", shell=True, stdout=subprocess.PIPE).stdout
hostos = pipe.readlines()[0].rstrip(os.linesep)
pipe.close()
nr_cores=0
if hostos == "FreeBSD":
        pipe = subprocess.Popen("sysctl -n hw.ncpu", shell=True, stdout=subprocess.PIPE).stdout
        nr_cores = int(pipe.readlines()[0].rstrip(os.linesep))
elif hostos == "Linux":
        r = re.compile('^processor')
        fd = open('/proc/cpuinfo', 'r')
        for line in fd.readlines():
	        if r.search(line):
		        nr_cores = nr_cores + 1
        fd.close()
        setarch = 'setarch linux64 -R'
        try:
	        retcode = subprocess.call(setarch + " /bin/true", shell=True)
        except OSError, e:
	        retcode = -1

        if retcode != 0:
	        setarch = ''
	        print >> sys.stderr, 'WARNING: setarch -R failed, address space randomization may cause variability'

print 'tasks,processes,processes_idle,threads,threads_idle,linear'
print '0,0,100,0,100,0'

step = 4
# if step=4, this is: [4, 8, 12, ... nr_cores]
data_points = range(step, nr_cores+step, step)
# this makes it [ 1, 4, 8, ... ]
if step > 1:
	data_points.insert(0, 1)

for i in data_points:
	c = './%s_processes -t %d -s %d' % (cmd, i, duration)
	before = wis_stat()
	pipe = subprocess.Popen(c, shell=True, stdout=subprocess.PIPE).stdout
	processes_avg = -1
	for line in pipe.readlines():
		if 'testcase:' in line:
			(testcase, val) = line.split(':')
			title = open(cmd + '.title', 'w')
			title.write(val)	
			title.close() 
			
		if 'average:' in line:
			(name, val) = line.split(':')
			processes_avg = int(val)
	pipe.close()
	after = wis_stat()
	processes_idle = after.idle_fraction(before) * 100

	c = './%s_threads -t %d -s %d' % (cmd, i, duration)
	before = wis_stat()
	pipe = subprocess.Popen(c, shell=True, stdout=subprocess.PIPE).stdout
	threads_avg = -1
	for line in pipe.readlines():
		if 'average:' in line:
			(name, val) = line.split(':')
			threads_avg = int(val)
	pipe.close()
	after = wis_stat()
	threads_idle = after.idle_fraction(before) * 100

	if i == 1:
		linear = max(processes_avg, threads_avg)

	print '%d,%d,%0.2f,%d,%0.2f,%d' % (i, processes_avg, processes_idle, threads_avg, threads_idle, linear * i)
