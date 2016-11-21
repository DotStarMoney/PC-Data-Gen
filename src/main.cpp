#include "context.h"
#include "image.h"
#include "gfxfunctions.h"
#include "vec2.h"
#include "SDL.h"


int main(int _argc, char** _argv)
{
	pc::Context scn(640, 480);
	pc::Image buffer(640, 480);
	
	scn.sync(buffer);

	

	return 0;
}
