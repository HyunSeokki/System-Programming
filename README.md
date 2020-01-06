System-Programming
===============

HW1 ~ HW3
=========
기본적인 능력을 수행해내는 가상의 컴퓨터인 SIC/XE Machine의 Assembly Code에 대한 Assembler, Linking loader입니다.
HW3이 완성된 프로젝트이며, 몇 가지 명령어를 수행하는 Shell이 동작합니다.

#### How to compile the code

	$ make

#### How to run

	$ ./sicsim.out

#### How to clean the file

	$ make clean

#### Shell Command
	
	h[elp]
	d[ir]
	q[uit]
	hi[story]
	du[mp] [start, end]
	e[dit] address, value
	f[ill] start, end, value
	reset
	opcode mnemonic
	opcodelist
	assemble filename
	type filename
	symbol
	progaddr [address]
	loader [object filename1] [object filename2] [...]
	run
	bp [address/clear]

HW4
=====
Python3와 BeautifulSoup4, Requests를 사용하여 해당 웹사이트의 모든 하이퍼링크를 재귀적으로 방문하여 모든 페이지의 text를 수집합니다.

HW5
=====
AWS의 Hadoop 시스템을 이용하여 N-Gram 모델을 생성합니다.
N-Gram 언어모델은 현재 단어로부터 앞의 n-1개의 단어가 주어졌을때, 현재의 단어가 생성되는 확률을 계산해주는 모델입니다.

HW6
=====
Azure ML을 이용하여 Machine Learning 모델을 구축합니다.