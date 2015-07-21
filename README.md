ChunlanAC Lib
==============

An Arduino Lib for Chunlan Air Condition

Prerequisite:
-------------
1)	it use IRLib http://tech.cyborg5.com/irlib/
2)	_Update 21-Jul-2015:_ No need for C++11 Changes



The code is provided under The MIT License (MIT)

Install:
--------
```
Place the lib in 'libraries' sub folder of your Arduino sketch folder
e.g. "C:\MyProjects\Arduino\libraries\ChunlanAC"
```

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
	
Notes:
------
Picture of the AC Remote:
![Chunlan AC Remote](https://github.com/baget/ChunlanAc/raw/master/Docs/ACRemote.jpg "Chunlan AC Remote")

	
Special Thanks:
--------------
Thanks to Chris from analysIR for this support on this project
http://www.analysir.com/