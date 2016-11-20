#pragma once
#include "vec2.h"

namespace pc
{
	class body
	{
	public:
		body();
		~body();
	protected:
		vec2 p;
		vec2 v;
	};
}