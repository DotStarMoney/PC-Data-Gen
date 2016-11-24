#include "context.h"
#include "term.h"
#include "SDL.h"
#include <string.h>
#include <stdexcept>


namespace pc
{
	CALL_ON_TERMINATION(CONTEXT_CPP, SDL_Quit);

	bool has_init_sdl = false;

	Context::Context()
	{
		construct();

		fullscreen = Default::fullscreen;
		width = Default::width;
		height = Default::height;

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
	Context::Context(size_t _w, size_t _h, bool _fullscreen)
	{
		construct();

		fullscreen = _fullscreen;
		width = _w;
		height = _h;

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


	Context::~Context()
	{
		if (window) SDL_DestroyWindow(window);
		delete(title);
	}
	void Context::construct()
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
		fullscreen.P_ASSIGN(&Context::fullscreen_changed);
		width.P_ASSIGN(&Context::width_changed);
		height.P_ASSIGN(&Context::height_changed);
	}

	void Context::sync(const Image& _img)
	{
		SDL_Surface* scn;
		scn = SDL_GetWindowSurface(window);
		SDL_BlitSurface(_img.surface, nullptr, scn, nullptr);
		SDL_UpdateWindowSurface(window);
	}
	void Context::fullscreen_changed(bool _fullscreen)
	{
		fullscreen.value = _fullscreen;
		SDL_SetWindowFullscreen(
			window,
			fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	}
	void Context::title_changed(char* _title)
	{
		size_t string_size;

		delete(title.value);
		string_size = strlen(_title);
		title.value = new char[string_size + 1];
		strcpy_s(title.value, string_size + 1, _title);

		SDL_SetWindowTitle(window, title);
	}
	void Context::width_changed(size_t _width)
	{
		width.value = _width;
		SDL_SetWindowSize(window, width, height);
	}
	void Context::height_changed(size_t _height)
	{
		height.value = _height;
		SDL_SetWindowSize(window, width, height);
	}
	void Context::set_size(size_t _w, size_t _h)
	{
		width.value = _w;
		height.value = _h;
		SDL_SetWindowSize(window, width, height);
	}


}