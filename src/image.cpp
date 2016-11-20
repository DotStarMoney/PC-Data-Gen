#include "image.h"
#include "SDL.h"
#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include <stdexcept>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif

namespace pc
{
	void Image::construct()
	{
		surface = nullptr;
	}
	Image::Image()
	{
		construct();
	}
	Image::Image(const std::string& _file)
	{
		construct();
		load(_file);
	}
	Image::Image(size_t _width, size_t _height)
	{
		clear();
		surface = SDL_CreateRGBSurface(
			0, 
			_width, 
			_height, 
			32,
			rmask, gmask, bmask, amask);
		if(!surface) throw std::runtime_error("Failed to create image.");
	}
	Image::~Image()
	{
		if (surface) SDL_FreeSurface(surface);
	}

	void Image::load(const std::string& _file)
	{
		clear();
		surface = STBIMG_Load(_file.c_str());
		if (!surface) throw std::runtime_error("Failed to load image.");
	}
	
	void Image::clear()
	{
		if (surface) SDL_FreeSurface(surface);
		surface = nullptr;
	}

}