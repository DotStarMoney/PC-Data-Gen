#include "context.h"
#include "term.h"
#include "SDL.h"
#include <string.h>
#include <stdexcept>

//flesh out context

namespace pc
{
	CALL_ON_TERMINATION(SDL_Quit)

	bool has_init_sdl = false;
	Context::Context()
	{
		size_t string_size;
		if (!has_init_sdl)
		{
			has_init_sdl = true;
			SDL_Init(SDL_INIT_VIDEO);
		}
		window = nullptr;

		string_size = strlen(Default::title);
		title = new char[string_size + 1];
		strcpy_s(title, string_size + 1, Default::title);
		title.assign(this, &Context::title_changed);

		fullscreen = Default::fullscreen;
		fullscreen.assign(this, &Context::fullscreen_changed);

		width = Default::width;
		width.assign(this, &Context::width_changed);

		height = Default::height;
		height.assign(this, &Context::height_changed);
	}

	Context::~Context()
	{
		if (window) SDL_DestroyWindow(window);
		delete(title);
	}

	void Context::sync()
	{
		if (!window)
		{
			window = SDL_CreateWindow(
				title,
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width,
				height,
				fullscreen ? SDL_WINDOW_FULLSCREEN : 0
				);
			if (!window) throw std::runtime_error("Could not open context.");
		}


	}
	void Context::fullscreen_changed(bool _fullscreen)
	{

	}
	void Context::title_changed(char* _title)
	{

	}
	void Context::width_changed(size_t _width)
	{

	}
	void Context::height_changed(size_t _height)
	{

	}

}