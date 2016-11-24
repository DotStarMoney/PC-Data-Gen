#pragma once
#include <stddef.h>
#include "vec2.h"
#include <string>
#include "color32.h"


namespace pc
{
	class Image;

	namespace gfx
	{
		enum class LineCode { L, B, BF };
		enum class CircleCode { L, F };
		enum class MethodCode { NONE, ALPHA, ADD, MIX};

		void cls(const Image& _image, Color32 _col = 0x00000000);
		void pset(const Image& _image, vec2 _p, Color32 _col);
		Color32 point(const Image& _image, vec2 _p);
		void line(
			const Image& _image,
			vec2 _s,
			vec2 _e,
			Color32 _col = Color32(0xffffffff),
			LineCode _code = LineCode::L);
		void circle(
			const Image& _image,
			vec2 _p,
			float _r,
			Color32 _col = Color32(0xffffffff),
			CircleCode _code = CircleCode::L);
		void put(
			Image& _dest,
			vec2 _p,
			Image& _src,
			MethodCode _method = MethodCode::NONE,
			vec2 _src_s = vec2(-1, -1),
			vec2 _src_e = vec2(-1, -1),
			uint8_t _alpha = 255);
		void putf(
			Image& _dest,
			vec2 _p,
		    Image& _src,
			MethodCode _method = MethodCode::NONE,
			vec2 _src_s = vec2(-1, -1),
			vec2 _src_e = vec2(-1, -1),
			uint8_t _alpha = 255);
		void draw_text(
			const Image& _dest,
			vec2 _p,
			std::string _text,
			Color32 _col = Color32(0xffffffff));
	}
}