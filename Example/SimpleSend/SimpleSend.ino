/*
	Library for Chunlan A.C. 
	SimpleSend Example
	
	
	Author: Oren Weil
	
	The MIT License (MIT)

	Copyright (c) 2015 Oren Weil

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <IRLib.h>
#include <ChunlanAc.h>

using namespace org::baget;

int khz = 38; 

IRsend irSender;
IrEncoder encoder;
AcCmd cmd;

unsigned int outArr[256];
size_t outArrSize = 0;


void setup() {
  memset(&outArr, 0, sizeof(outArr));
}

void loop() {  
  cmd.setState(AcCmd::State::On);
  cmd.setMode(AcCmd::Mode::Cool, 0);
  cmd.setTemp(25);
  
  encoder.encode(cmd, outArr, &outArrSize);
  irSender.IRsendRaw::send(outArr, outArrSize, khz); 

  delay(3000);
  
  cmd.setState(AcCmd::State::Off);
  cmd.setMode(AcCmd::Mode::Cool, 0);
  cmd.setTemp(25);
  
  encoder.encode(cmd, outArr, &outArrSize);
  irSender.IRsendRaw::send(outArr, outArrSize, khz); 
  
  delay(3000);
}
