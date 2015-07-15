ChunlanAC Lib
==============

An Arduino Lib for Chunlan Air Condition

Prerequisite:
-------------
1)	it was written in C++11
	so you need to Modify your Arduino platform.txt
	e.g. in _C:\Program Files (x86)\Arduino\hardware\arduino\avr\platform.txt_

	Add to "compiler.cpp.flags" -std=c++11 in the end.

2) 	it use IRLib http://tech.cyborg5.com/irlib/

The code is provided under The MIT License (MIT)

Install:
--------
Place the lib in 'libraries' sub folder of your Arduino sketch folder
e.g. "C:\MyProjects\Arduino\libraries\ChunlanAC"

Protocol:
---------
I reversed engineered the IR Protocol of the AC and found the following is the format:
it based of sending the same DWORD (4 Bytes) three Times,
the 2nd time is negative value (e.g. ACACACAC 53535353 ACACACAC)

Byte	| Bits	| Meaning		| Values	|
:------:|:-----:|:-------------:|:---------:|
3		| 0:7	| Header		| 0xB3		|
2		| 0:7	| Header		| 0x35		|
1		| 7		| Const			| Always 1	|
1		| 4:6	| State			| On = 010, Off = 101, Cmd = 001	|
1		| 0:3	| Temp			| 17=0000, 18=1000, 19=0100, ..., 23=0110, 24=1110, 25=0001, 26=1001, 27=0101, 28=1101	|
0		| 4:7	| Mode and Fan	| Cool 1:1110, 2:0001, 3:1001, Auto:0110 Dehumidif: 1010 Fan Only: 0101 Heat 1:0111, 2:1111, 3:0000 Auto:1011	|
0		| 0:3	| Const			| Always 0010	|
	
Special Thanks:
--------------
Thanks to Chris from analysIR for this support on this project
http://www.analysir.com/