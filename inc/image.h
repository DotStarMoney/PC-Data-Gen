#pragma once
#include <string>
#include <stddef.h>
#include "noncopyable.h"

struct SDL_Surface;
namespace pc
{
	class Image
	{
	public:
		NONCOPYABLE(Image)
		Image();
		Image(const std::string& _file);
		Image(size_t _width, size_t _height);
		~Image();
		void load(const std::string& _file);
	private:
		void construct();
		void clear();
	private:
		SDL_Surface* surface;
	};

}