#pragma once
#include <string>
#include <stddef.h>
#include "noncopyable.h"
#include "parameter_ro.h"
#include "parameter.h"
#include <stdint.h>
#include "color32.h"
#include "gfxfunctions.h"

struct SDL_Surface;
namespace pc
{

	class Image
	{
		friend void gfx::cls(const Image& _image, Color32 _col);
		friend void gfx::line(
			const Image& _image,
			vec2 _s,
			vec2 _e,
			Color32 _col,
			LineCode _code);
		friend void gfx::put(
			Image& _dest,
			vec2 _p,
			Image& _src,
			MethodCode _method,
			vec2 _src_s,
			vec2 _src_e,
			uint8_t _alpha);
		friend void gfx::putf(
			Image& _dest,
			vec2 _p,
			Image& _src,
			MethodCode _method,
			vec2 _src_s,
			vec2 _src_e,
			uint8_t _alpha);
		friend Context;
	public:
		NONCOPYABLE(Image)
		Image();
		Image(const std::string& _file);
		Image(size_t _width, size_t _height);
		~Image();
		void load(const std::string& _file);
		void enable_colorkey(bool _enable);
		bool empty();
		void compress();
	public:
		Parameter<Color32, Image> colorkey;
		Parameter_RO<size_t, Image> pitch;
		Parameter_RO<void*, Image> data;
		Parameter_RO<size_t, Image> width;
		Parameter_RO<size_t, Image> height;
	private:
		void construct();
		void clear();
		void colorkey_changed(Color32 _col);
		void pitch_read();
		void data_read();
		void width_read();
		void height_read();
		void* get_raw_data();
	private:
		bool explicit_keying;
		bool enabled_keying;
		bool is_rle;
		SDL_Surface* surface;
	};

}