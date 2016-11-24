#include "utility.h"
#include "SDL.h"
#include "term.h"
#include <time.h>
#include <stdlib.h>

namespace pc
{
	void seed();
	CALL_ON_START(UTILITY_H, seed);
	void seed()
	{
		srand(time(NULL));
	}

	void seed(unsigned int _s)
	{
		srand(_s);
	}
	void sleep(unsigned int _t)
	{
		SDL_Delay(_t);
	}
	float rnd()
	{
		return (float)rand() / RAND_MAX;
	}
}
