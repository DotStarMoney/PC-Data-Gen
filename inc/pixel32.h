#pragma once
#include <stdint.h>

union Pixel32
{
	Pixel32(uint32_t _v) { value = _v; }
	Pixel32(
		unsigned char _r,
		unsigned char _g,
		unsigned char _b,
		unsigned char _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	void operator=(const uint32_t _val) { value = _val; }
	struct
	{
		unsigned char r : 8;
		unsigned char g : 8;
		unsigned char b : 8;
		unsigned char a : 8;
	};
	uint32_t value;
};