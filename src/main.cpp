#include "context.h"
#include "image.h"
#include "gfxfunctions.h"
#include "vec2.h"
#include "utility.h"

using namespace pc;
int main(int _argc, char** _argv)
{
	Context scn(640, 480);
	Image buffer(640, 480);
	Image pic(64, 64);
	Color32 col;
	
	gfx::cls(buffer, 0xED9564);

	int i;
	for (i = 6; i >= 0; i--)
	{
		col = Color32((i & 1) * 223 + 32, (i & 1) * 255, 128);
		gfx::circle(pic, vec2(16, 16), i * 10 + 5, col, gfx::CircleCode::F);
	}
	gfx::line(pic, vec2(0, 0), vec2(pic.width - 1, pic.height - 1), 0, gfx::LineCode::B);
	pic.compress();

	pic.colorkey = Color32(255, 255, 128);

	gfx::line(buffer, vec2(13, 10), vec2(175, 101), Color32(20, 20, 20), gfx::LineCode::BF);
	gfx::line(buffer, vec2(13, 10), vec2(175, 101), Color32(255, 255, 255), gfx::LineCode::B);
	gfx::line(buffer, vec2(13, 10), vec2(175, 101), Color32(255, 0, 0), gfx::LineCode::L);


	for (i = 0; i < 20; i++)
	{
		gfx::put(
			buffer, 
			vec2(
				rnd() * (scn.width - pic.width), 
				rnd() * (scn.height - pic.height)), 
			pic,
			gfx::MethodCode::MIX);
	}

	scn.sync(buffer);
	
	sleep(50000);

	return 0;
}
