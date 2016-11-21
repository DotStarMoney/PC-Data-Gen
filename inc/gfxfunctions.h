#pragma once
#include <stddef.h>
#include "vec2.h"
#include <string>
#include "pixel32.h"


namespace pc
{
	class Image;

	namespace gfx
	{
		enum class LineCode { L, B, BF };
		enum class CircleCode { L, F };
		enum class MethodCode { NONE, ALPHA, ADD, MIX};

		void cls(const Image& _image, Pixel32 _col = 0x00000000);
		void pset(const Image& _image, vec2 _p, Pixel32 _col);
		Pixel32 point(const Image& _image, vec2 _p);
		void line(
			const Image& _image,
			vec2 _s,
			vec2 _e,
			LineCode _code,
			Pixel32 _col);
		void circle(
			const Image& _image,
			vec2 _p,
			float _r,
			CircleCode _code,
			Pixel32 _col);
		void put(
			Image& _dest,
			vec2 _p,
			Image& _src,
			MethodCode _method,
			ivec2 _src_s = ivec2(-1, -1),
			ivec2 _src_e = ivec2(-1, -1),
			uint8_t _alpha = 255);
		void putf(
			Image& _dest,
			vec2 _p,
		    Image& _src,
			MethodCode _method,
			ivec2 _src_s = ivec2(-1, -1),
			ivec2 _src_e = ivec2(-1, -1),
			uint8_t _alpha = 255);
		void draw_text(
			const Image& _dest,
			vec2 _p,
			std::string _text,
			Pixel32 _col);
	}
}