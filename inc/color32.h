#pragma once
#include <stdint.h>

union Color32
{
	Color32() {}
	Color32(uint32_t _v) { value = _v; }
	Color32(
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
	Color32(
		unsigned char _r,
		unsigned char _g,
		unsigned char _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = 0xff;
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