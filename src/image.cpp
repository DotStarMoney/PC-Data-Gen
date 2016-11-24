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
		colorkey = 0xffff00ff;
		explicit_keying = false;
		enabled_keying = false;
		is_rle = false;
		colorkey.P_ASSIGN(&Image::colorkey_changed);
		pitch.P_ASSIGN(&Image::pitch_read);
		data.P_ASSIGN(&Image::data_read);
		width.P_ASSIGN(&Image::width_read);
		height.P_ASSIGN(&Image::height_read);
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
		construct();
		clear();
		surface = SDL_CreateRGBSurface(
			0,
			_width,
			_height,
			32,
			rmask, gmask, bmask, amask);
		if (!surface) throw std::runtime_error("Failed to create image.");
		SDL_SetSurfaceRLE(surface, 1);
		is_rle = true;
		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
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
		SDL_SetSurfaceRLE(surface, 1);
		is_rle = true;
		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
	}

	void Image::clear()
	{
		if (surface) SDL_FreeSurface(surface);
		surface = nullptr;
	}

	void Image::pitch_read()
	{
		if(!surface) throw std::runtime_error("No image.");
		pitch = surface->pitch;
	}
	void Image::data_read()
	{
		if (!surface) throw std::runtime_error("No image.");
		SDL_SetSurfaceRLE(surface, 0);
		is_rle = false;
		data = surface->pixels;
	}
	void Image::width_read()
	{
		if (!surface) throw std::runtime_error("No image.");
		width = surface->w;
	}
	void Image::height_read()
	{
		if (!surface) throw std::runtime_error("No image.");
		height = surface->h;
	}
	void Image::colorkey_changed(Color32 _col)
	{
		colorkey.value = _col;
		if (!surface) throw std::runtime_error("No image.");
		if (explicit_keying)
		{
			SDL_SetColorKey(surface, enabled_keying, _col.value);
		}
		else
		{
			SDL_SetColorKey(surface, true, _col.value);
		}
	}
	void Image::enable_colorkey(bool _enable)
	{
		explicit_keying = true;
		enabled_keying = _enable;
	}
	bool Image::empty()
	{
		if (surface) return false;
		return true;
	}
	void Image::compress()
	{
		if (!surface) throw std::runtime_error("No image.");
		SDL_SetSurfaceRLE(surface, 1);
		is_rle = true;
	}

	void* Image::get_raw_data()
	{
		if (!surface) throw std::runtime_error("No image.");
		return data.value;
	}
}
