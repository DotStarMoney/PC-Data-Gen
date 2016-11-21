#pragma once
#include <string>
#include <stddef.h>
#include "noncopyable.h"
#include "parameter_ro.h"
#include <stdint.h>
#include "pixel32.h"
#include "gfxfunctions.h"

struct SDL_Surface;
namespace pc
{

	class Image
	{
		friend void gfx::cls(const Image& _image, Pixel32 _col);
		friend void gfx::line(
			const Image& _image,
			vec2 _s,
			vec2 _e,
			LineCode _code,
			Pixel32 _col);
	public:
		NONCOPYABLE(Image)
		Image();
		Image(const std::string& _file);
		Image(size_t _width, size_t _height);
		~Image();
		void load(const std::string& _file);
	public:
		Parameter_RO<size_t, Image> pitch;
		Parameter_RO<void*, Image> data;
		Parameter_RO<size_t, Image> width;
		Parameter_RO<size_t, Image> height;
	private:
		void construct();
		void clear();
		void pitch_read();
		void data_read();
		void width_read();
		void height_read();
	private:
		SDL_Surface* surface;
	};

}