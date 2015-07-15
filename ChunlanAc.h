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

#ifndef __CHUNLANAC_H
#define __CHUNLANAC_H

namespace org {
	namespace baget {
	
#include <stdint.h>


// from: https://stackoverflow.com/questions/4415524/common-array-length-macro-for-c
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))


uint8_t reverseBits(uint8_t b);

#pragma pack(1)
struct AcCmd {
	union{
		struct {
			uint8_t const1 : 4;
			uint8_t mode : 4;
			uint8_t temp : 4;
			uint8_t state : 3;
			uint8_t const2 : 1;
			uint8_t header2;
			uint8_t header1;
		} fields;

		uint32_t raw;
	};

	static const uint8_t HEADER1 = 0xB3;
	static const uint8_t HEADER2 = 0x35;

	AcCmd()
	{
		fields.header1 = HEADER1;
		fields.header2 = HEADER2;
		fields.const1 = 0x02; // 0b0010
		fields.const2 = 1;

	}

	uint8_t getTemp() const
	{
		return TEMP_BASE + (reverseBits(fields.temp) >> 4);
	}

	bool setTemp(uint8_t value)
	{
		if (value < 17 || value > 33) {
			//throw out_of_range("Temp value is out of range");
			return false;
		}

		uint8_t temp = reverseBits(value - TEMP_BASE) >> 4;

		fields.temp = temp & 0x0f;

		return true;
	}


	const char* getState() const
	{
		return States[fields.state];
	}


	enum class State { Cmd = 0x01, On = 0x02, Off = 0x05 };
	

	/*
		State - AC Command (Command, On or Off)
		return bool - true was able to set the mode
	*/

	void setState(State state)
	{
		fields.state = static_cast<uint8_t>(state);
	}

	const char* getMode() const
	{
		return ModesStrings[fields.mode];
	}


	enum class Mode { Cool, Heat, Dehumidif, FanOnly };

	/*
		Mode - AC Mode (Cool/Heat/Dehumidif/FanOnly)
		Fan - Fan Power (Fan: 0 = Auto, 1 = Fan 1, 2 = Fan 2, 3 = Fan 3)

		return bool - true was able to set the mode
	*/
	bool setMode(Mode mode, uint8_t fan)
	{
		uint8_t value = 0xFF;

		if (fan > 3) {
			//	throw out_of_range("fan value is out of range");
			return false;
		}

		value = Modes[static_cast<uint8_t>(mode)][fan];

		if (value == 0xFF) {
			//throw exception("In Valid Combintion"); 
			return false;
		}

		fields.mode = value;
		return true;
	}

private:
	static const uint8_t TEMP_BASE = 17;
	static const char* States[];
	static const char* ModesStrings[];

	static const uint8_t Modes[4][4];
};
#pragma pack()


class IrEncoder {
private:
	static const unsigned int Header[];
	static const unsigned int Footer[];

	static const unsigned int One[];
	static const unsigned int Zero[];


	IrEncoder& operator=(const IrEncoder&) = delete;

	void encodeuint32(uint32_t raw, unsigned int *outData, size_t *outCount);

	void encodeheader(unsigned int *outData, size_t *outCount);
	void encodefooter(unsigned int *outData, size_t *outCount);

public:
	IrEncoder() {

	}

	bool encode(const AcCmd& cmd, unsigned int *outData, size_t *outCount);

	
};



}
}

#endif
