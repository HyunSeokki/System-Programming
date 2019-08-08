#!/usr/bin/python3
# -*- coding: utf-8 -*-

from sys import stdin, stdout

lib = {}

for line in stdin:
	words, cnt = line.split('\t')
	if words not in lib:
		lib[words] = int(cnt)
	else:
		lib[words] += int(cnt)

for words in sorted(lib):
	stdout.write(words + '\t' + str(lib[words]) + '\n')
