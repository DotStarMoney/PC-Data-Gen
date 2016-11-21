#include "gfxfunctions.h"
#include "image.h"
#include "SDL.h"
#include <math.h>
#include <stdexcept>
#include <algorithm>

#define PRECISION 0.0001
#define M_EFFECTIVE_ZERO(_X_) ((_X_) < PRECISION ? true : false)

namespace pc
{
	namespace gfx
	{
		int get_code(float _x, float _y, float _w, float _h) 
		{
			int code;
			code = 0;
			if (_y > _h) code |= 8;
			if (_y < 0.0)  code |= 4;
			if (_x < 0.0)  code |= 1;
			if (_x > _w) code |= 2;
			return code;
		}
		bool adjust_line(vec2& _s, vec2& _e, float _w, float _h)
		{
#define MAX_ITERS 1000
			float m;
			float x;
			float y;
			int i;
			int t;
			vec2 d;
			int out_code1;
			int out_code2;
			out_code1 = get_code(_s.x, _s.y, _w, _h);
			out_code2 = get_code(_e.x, _e.y, _w, _h);
			d = _e - _s;
			if (d.x > d.y)
			{
				for (i = 0; i < MAX_ITERS; i++) {
					m = (_e.y - _s.y) / (_e.x - _s.x);
					if ((out_code1 == 0) && (out_code2 == 0))
					{
						return true;
					}
					else if ((out_code1 & out_code2) != 0)
					{
						return false;
					}
					else
					{
						if (out_code1 == 0)
						{
							t = out_code2;
						}
						else
						{
							t = out_code1;
						}
						if (t & 8)
						{
							x = _s.x + (_h - _s.y) / m;
							y = _h;
						}
						else if (t & 4)
						{ 
							x = _s.x - _s.y / m;
							y = 0;
						}
						else if (t & 1)
						{
							x = 0;
							y = _s.y - _s.x * m;
						}
						else if (t & 2) 
						{
							x = _h;
							y = _s.y + (_w - _s.x) * m;
						}
						if (t == out_code1) {
							_s.x = x;
							_s.y = y;
							out_code1 = get_code(_s.x, _s.y, _w, _h);
						}
						else 
						{
							_e.x = x;
							_e.y = y;
							out_code2 = get_code(_e.x, _e.y, _w, _h);
						}
					}
				}
			}
			else
			{
				for (i = 0; i < MAX_ITERS; i++) {
					m = (_e.x - _s.x) / (_e.y - _s.y);
					if ((out_code1 == 0) && (out_code2 == 0))
					{
						return true;
					}
					else if ((out_code1 & out_code2) != 0)
					{
						return false;
					}
					else
					{
						if (out_code1 == 0)
						{
							t = out_code2;
						}
						else
						{
							t = out_code1;
						}
						if (t & 8)
						{
							x = _s.x + (_h - _s.y) * m;
							y = _h;
						}
						else if (t & 4)
						{
							x = _s.x - _s.y * m;
							y = 0;
						}
						else if (t & 1)
						{
							x = 0;
							y = _s.y - _s.x / m;
						}
						else if (t & 2)
						{
							x = _h;
							y = _s.y + (_w - _s.x) / m;
						}
						if (t == out_code1) {
							_s.x = x;
							_s.y = y;
							out_code1 = get_code(_s.x, _s.y, _w, _h);
						}
						else
						{
							_e.x = x;
							_e.y = y;
							out_code2 = get_code(_e.x, _e.y, _w, _h);
						}
					}
				}
			}
			throw std::runtime_error("Iterations maxed out.");
		}
		void draw_horiz_line(
			void* _d,
			int _pitch,
			float _y,
			float _xs,
			float _xe,
			float _w,
			float _h,
			Pixel32 _col)
		{
			int xi;
			int yi;
			int xend;
			int offset;
			if (_y < 0) return;
			if (_y >= _h) return;
			if (_xe < _xs) std::swap(_xs, _xe);
			if (_xe < 0) return;
			if (_xs >= _w) return;
			if (_xe > _w) _xe = _w;
			if (_xs < 0) _xs = 0;
			xi = (int)_xs;
			yi = (int)_y;
			xend = (int)_xe;
			offset = xi * 4 + yi * _pitch;
			while (xi <= xend)
			{
				*((Pixel32*)&((uint8_t*)_d)[offset]) = _col;
				offset += 4;
				xi++;
			}
		}
		void draw_vert_line(
			void* _d,
			int _pitch,
			float _x,
			float _ys,
			float _ye,
			float _w,
			float _h,
			Pixel32 _col)
		{
			int xi;
			int yi;
			int yend;
			int offset;
			if (_x < 0) return;
			if (_x >= _w) return;
			if (_ye < _ys) std::swap(_ys, _ye);
			if (_ye < 0) return;
			if (_ys >= _h) return;
			if (_ye > _h) _ye = _h;
			if (_ys < 0) _ys = 0;
			xi = (int)_x;
			yi = (int)_ys;
			yend = (int)_ye;
			offset = xi * 4 + yi * _pitch;
			while (yi <= yend)
			{
				*((Pixel32*)&((uint8_t*)_d)[offset]) = _col;
				offset += _pitch;
				yi++;
			}
		}


		void cls(const Image& _image, Pixel32 _col)
		{
			SDL_FillRect(_image.surface, NULL, _col.value);
		}
		void pset(const Image& _image, vec2 _p, Pixel32 _col)
		{
			int x;
			int y;
			void* d;
			x = (int)_p.x;
			y = (int)_p.y;
			if ((x < 0) ||
				(y < 0) ||
				(x >= _image.width) ||
				(y >= _image.height)) return;
			d = _image.data;
			*((Pixel32*) &((uint8_t*)d)[x * 4 + y*_image.pitch]) = _col;
		}

		Pixel32 point(const Image& _image, vec2 _p)
		{
			int x;
			int y;
			void* d;
			x = (int)_p.x;
			y = (int)_p.y;
			if ((x < 0) ||
				(y < 0) ||
				(x >= _image.width) ||
				(y >= _image.height)) return -1;
			d = _image.data;
			return *((Pixel32*)&((uint8_t*)d)[x * 4 + y*_image.pitch]);
		}
		void line(
			const Image& _image,
			vec2 _s,
			vec2 _e,
			LineCode _code,
			Pixel32 _col)
		{
			int w;
			int h;
			int xi;
			int yi;
			int yend;
			int xend;
			int offset;
			int pitch;
			float slope;
			void* d;
			SDL_Rect fill_r;
			vec2 dt;
			w = _image.width;
			h = _image.height;
			pitch = _image.pitch;
			dt = _e - _s;
			if (_code == LineCode::L)
			{
				d = _image.data;
				if (M_EFFECTIVE_ZERO(abs(dt.x)))
				{
					if (M_EFFECTIVE_ZERO(abs(dt.y)))
					{
						pset(_image, _s, _col);
						return;
					}
					draw_vert_line(d, pitch, _s.x, _s.y, _e.y, w, h, _col);
					return;
				}
				else if (M_EFFECTIVE_ZERO(abs(dt.y)))
				{
					draw_horiz_line(d, pitch, _s.y, _s.x, _e.x, w, h, _col);
					return;
				}
				else
				{
					if (adjust_line(
						_s, 
						_e, 
						(float) w - PRECISION, 
						(float) h - PRECISION))
					{
						if (dt.x > dt.y)
						{
							slope = dt.y / dt.x;
							while (_s.x < _e.x)
							{
								xi = (int)_s.x;
								yi = (int)_s.y;
								*((Pixel32*)&((uint8_t*)d)
									[xi * 4 + yi*_image.pitch]) = _col;
								_s.x += 1;
								_s.y += slope;
							}
						}
						else
						{
							slope = dt.x / dt.y;
							while (_s.y < _e.y)
							{
								xi = (int)_s.x;
								yi = (int)_s.y;
								*((Pixel32*)&((uint8_t*)d)
									[xi * 4 + yi*_image.pitch]) = _col;
								_s.x += slope;
								_s.y += 1;
							}
						}
						xi = (int)_e.x;
						yi = (int)_e.y;
						*((Pixel32*)&((uint8_t*)d)
							[xi * 4 + yi*_image.pitch]) = _col;
					}
					return;
				}
			}
			else if (_code == LineCode::B)
			{
				d = _image.data;
				draw_horiz_line(d, pitch, _s.y, _s.x, _e.x, w, h, _col);
				draw_horiz_line(d, pitch, _e.y, _s.x, _e.x, w, h, _col);
				draw_vert_line(d, pitch, _s.x, _s.y, _e.y, w, h, _col);
				draw_vert_line(d, pitch, _e.x, _s.y, _e.y, w, h, _col);
			}
			else if (_code == LineCode::BF)
			{
				fill_r.x = (int)_s.x;
				fill_r.y = (int)_s.y;
				fill_r.w = (int)abs(dt.x);
				fill_r.h = (int)abs(dt.y);
				SDL_FillRect(_image.surface, &fill_r, _col.value);
			}
		}

		void circle(
			const Image& _image,
			vec2 _p,
			float _r,
			CircleCode _code,
			Pixel32 _col)
		{

		}
		void put(
			const Image& _dest,
			vec2 _p,
			const Image& _src,
			MethodCode _method,
			ivec2 _src_s,
			ivec2 _src_e,
			Pixel32 _trans_col)
		{

		}
		void putf(
			const Image& _dest,
			vec2 _p,
			const Image& _src,
			MethodCode _method,
			ivec2 _src_s,
			ivec2 _src_e,
			Pixel32 _trans_col)
		{

		}
		void draw_text(
			const Image& _dest,
			vec2 _p,
			std::string _text,
			Pixel32 _col)
		{

		}

	}
}