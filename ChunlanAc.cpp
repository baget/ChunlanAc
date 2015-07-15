/*
	Library for Chunlan A.C. 
	
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

#include <stdint.h>
#include <string.h>
#include "ChunlanAc.h"


namespace org {
	namespace baget {

using namespace std;

const char* AcCmd::States[] = { "N/A", "CMD", "On", "N/A", "N/A", "Off", "N/A", "N/A" };
const char* AcCmd::ModesStrings[] = { 
	"Heat - Fan 3", "Cool - Fan 2", "N/A", "Fan Only 3", "N/A", "Fan Only 1", "Cool - Fan Auto",
	"Heat - Fan 1", "N/A", "Cool - Fan 3", "Dehumidif - Fan Auto", "Heat - Fan Auto", "N/A",
	"Fan Only 2", "Cool - Fan 1", "Heat - Fan 2" 
	};

const uint8_t AcCmd::Modes[4][4] =
{
	{ 0x06, 0x0E, 0x01, 0x09 },	// Cool
	{ 0x0b, 0x07, 0x0F, 0x00 },	// Heat
	{ 0x0A, 0xFF, 0xFF, 0xFF }, // Dehumidif
	{ 0xFF, 0x05, 0x0d, 0x03 },	// FanOnly
};

// Taken from 
// https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
uint8_t reverseBits(uint8_t b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}


const unsigned int IrEncoder::Header[]	= { 8341U, 3541U }; // 1 = 8341us 0 = 3541ms (Big Mark then smaller Space ) 
const unsigned int IrEncoder::Footer[]	= { 555U, 21077U }; // Small Mark then big space
const unsigned int IrEncoder::One[]		= { 555U, 1600U };   
const unsigned int IrEncoder::Zero[]	= { 555U, 491U }; 

bool IrEncoder::encode(const AcCmd& cmd, unsigned int *outData, size_t *outCount)
{
	size_t index = 0;

	if (!outCount || !outData)
	{
		return false;
	}


	// 1st DWORD - Normal
	encodeheader(outData, &index);
	encodeuint32(cmd.raw, outData, &index);
	encodefooter(outData, &index);

	// 2nd DWORD - Invert
	encodeheader(outData, &index);
	encodeuint32((~cmd.raw), outData, &index);
	encodefooter(outData, &index);

	// 3rd DWORD - Normal
	encodeheader(outData, &index);
	encodeuint32(cmd.raw, outData, &index);
	encodefooter(outData, &index);

	outData[index - 1] = 0;

	// Remove the last Space and return the size
	*outCount = index - 1;
		
	return true;
}


void IrEncoder::encodeuint32(uint32_t raw, unsigned int *outData, size_t *outCount)
{
	size_t bitCount = sizeof(raw) * 8;
	size_t index = *outCount;

	while (bitCount > 0)
	{
		if (raw & 0x80000000UL) {
			memcpy(static_cast<void*>(&outData[index]), One, sizeof(One));
		}
		else
		{
			memcpy(static_cast<void*>(&outData[index]), Zero, sizeof(Zero));
		}

		// Since COUNT_OF(One) == COUNT_OF(Zero);
		index += COUNT_OF(One);

		raw <<= 1;
		bitCount--;
	}

	*outCount = index;
}

inline void IrEncoder::encodeheader(unsigned int *outData, size_t *outCount)
{
	memcpy(static_cast<void*>(&outData[*outCount]), Header, sizeof(Header));
	*outCount += COUNT_OF(Header);

}

inline void IrEncoder::encodefooter(unsigned int *outData, size_t *outCount)
{
	memcpy(static_cast<void*>(&outData[*outCount]), Footer, sizeof(Footer));
	*outCount += COUNT_OF(Footer);
}



}
}

