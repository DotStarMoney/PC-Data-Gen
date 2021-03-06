#pragma once
#include <stddef.h>
#include "noncopyable.h"
#include "parameter.h"
#include "image.h"

struct SDL_Window;
namespace pc
{

	class Context
	{
	public:
		NONCOPYABLE(Context)
		Context();
		Context(size_t _w, size_t _h, bool _fullscreen = false);
		~Context();
		void sync(const Image& _img);
		void set_size(size_t _w, size_t _h);
		
	struct Default
	{
		static const size_t width = 640;
		static const size_t height = 480;
		static const bool fullscreen = false;
		static constexpr const char* const title = "Context";
	};
	public:
		Parameter<size_t, Context> width;
		Parameter<size_t, Context> height;
		Parameter<char*, Context> title;
		Parameter<bool, Context> fullscreen;
	private:
		SDL_Window* window;
		void construct();
		void fullscreen_changed(bool _fullscreen);
		void title_changed(char* _title);
		void width_changed(size_t _width);
		void height_changed(size_t _height);
	};
}