#include "context.h"
#include "image.h"
#include "gfxfunctions.h"
#include "vec2.h"


int main(int _argc, char** _argv)
{
	pc::Image a;

	pc::gfx::pset(a, pc::ivec2(10, 10), 0xffff0000);
	

	return 0;
}
