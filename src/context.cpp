#include "context.h"
#include "term.h"
#include "SDL.h"
#include <string.h>
#include <stdexcept>


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
		title.P_ASSIGN(&Context::title_changed);

		fullscreen = Default::fullscreen;
		fullscreen.P_ASSIGN(&Context::fullscreen_changed);

		width = Default::width;
		width.P_ASSIGN(&Context::width_changed);

		height = Default::height;
		height.P_ASSIGN(&Context::height_changed);
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
		fullscreen = _fullscreen;
		SDL_SetWindowFullscreen(
			window,
			fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	}
	void Context::title_changed(char* _title)
	{
		size_t string_size;

		delete(title);
		string_size = strlen(_title);
		title = new char[string_size + 1];
		strcpy_s(title, string_size + 1, _title);

		SDL_SetWindowTitle(window, title);
	}
	void Context::width_changed(size_t _width)
	{
		width = _width;
		SDL_SetWindowSize(window, width, height);
	}
	void Context::height_changed(size_t _height)
	{
		height = _height;
		SDL_SetWindowSize(window, width, height);
	}
	void Context::set_size(size_t _w, size_t _h)
	{
		width = _w;
		height = _h;
		SDL_SetWindowSize(window, width, height);
	}


}