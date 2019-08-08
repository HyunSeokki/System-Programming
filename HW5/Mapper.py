#!/usr/bin/python3
# -*- coding: utf-8 -*-

from sys import stdin, stdout

for line in stdin:
	line_split = line.split()
	for i in range(len(line_split) - 1):
               	stdout.write(line_split[i] + ' ' + line_split[i+1] + '\t1\n')
