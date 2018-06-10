#!/usr/local/bin/python

import time
import subprocess
import sys
import os
import re

if len(sys.argv) != 3:
    print >> sys.stderr, "Usage: cvsmerge.py a.csv b.csv"
    exit

left = sys.argv[1]
right = sys.argv[2]

leftf = open(left, 'r')
rightf = open(right, 'r')

leftl = leftf.readlines()
rightl = rightf.readlines()
outl = []
for line in leftf:
    leftl.append(line)
for line in rightf:
    rightl.append(line)

if len(leftl) < len(rightl):
    print >> sys.stderr, "{} shorter than {}".format(left, right)

print 'tasks,group1_processes,group1_threads,group1_linear,group2_processes,group2_threads,group2_linear'
print '0,0,0,0,0,0,0'

for i in range(2, len(leftl)):
    line = leftl[i].rstrip()
    lout = line.split(",")
    linear = lout[-1]
    lout = [lout[0], lout[1], lout[3], lout[5]]
    if i >= len(rightl):
        outl.append(lout.join(",") + ",0,0," + linear)
        continue
    rout = rightl[i].rstrip()
    rout = rout.split(",")
    rout = [rout[1], rout[3], rout[5]]
    outl.append(",".join(lout) + "," + ",".join(rout))

for line in outl:
    print line
