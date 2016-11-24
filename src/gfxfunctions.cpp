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
		bool any_clip(
			vec2 _p,
			vec2 _s,
			vec2 _dlb,
			vec2 _dhb,
			vec2& _dp,
			vec2& _clb,
			vec2& _chb)
		{
			vec2 p1;
			vec2 p2;
			vec2 bb1;
			vec2 bb2;

			p1 = _p;
			p2 = p1 + _s;
			
			if (p2.x <= _dlb.x) return false;
			if (p1.x >= _dhb.x) return false;
			if (p2.y <= _dlb.y) return false;
			if (p1.y >= _dhb.y) return false;

			bb1.x = p1.x < _dlb.x ? _dlb.x : p1.x;
			bb1.y = p1.y < _dlb.y ? _dlb.y : p1.y;
			bb2.x = p2.x > _dhb.x ? _dhb.x : p2.x;
			bb2.y = p2.y > _dhb.y ? _dhb.y : p2.y;

			_dp = bb1;
			_clb = bb1 - p1;
			_chb = _s - (p2 - bb2);

			return true;
		}

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
			Color32 _col)
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
				*((Color32*)&((uint8_t*)_d)[offset]) = _col;
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
			Color32 _col)
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
				*((Color32*)&((uint8_t*)_d)[offset]) = _col;
				offset += _pitch;
				yi++;
			}
		}


		void cls(const Image& _image, Color32 _col)
		{
			SDL_FillRect(_image.surface, NULL, _col.value);
		}
		void pset(const Image& _image, vec2 _p, Color32 _col)
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
			*((Color32*) &((uint8_t*)d)[x * 4 + y*_image.pitch]) = _col;
		}

		Color32 point(const Image& _image, vec2 _p)
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
			return *((Color32*)&((uint8_t*)d)[x * 4 + y*_image.pitch]);
		}
		void line(
			const Image& _image,
			vec2 _s,
			vec2 _e,
			Color32 _col,
			LineCode _code)
		{
			int w;
			int h;
			int xi;
			int yi;
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
								*((Color32*)&((uint8_t*)d)
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
								*((Color32*)&((uint8_t*)d)
									[xi * 4 + yi*_image.pitch]) = _col;
								_s.x += slope;
								_s.y += 1;
							}
						}
						xi = (int)_e.x;
						yi = (int)_e.y;
						*((Color32*)&((uint8_t*)d)
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
				fill_r.w = (int)abs(dt.x) + 1;
				fill_r.h = (int)abs(dt.y) + 1;
				SDL_FillRect(_image.surface, &fill_r, _col.value);
			}
		}

		void circle(
			const Image& _image,
			vec2 _p,
			float _r,
			Color32 _col,
			CircleCode _code)
		{
#define LOW_P_PI 3.141592
#define LOW_P_HPI 1.570796
			int r;
			int i;
			int xi;
			int yi;
			int ly;
			int lx;
			int llx;
			int oy;
			int ox;
			int olx;
			int lxi;
			int w;
			int h;
			int xs;
			int xe;
			int pitch;
			int offset;
			int mp;
			void* d;
			float x_p;
			float a;
			float a_d;
			w = _image.width;
			h = _image.height;
			d = _image.data;
			pitch = _image.pitch;
			r = (int)_r;
			a_d = 1.0 / _r;
			a = 0;
			ly = (int)_p.y - _r;
			lx = (int)_p.x;
			llx = (int)_p.x;
			i = 0;
			if (_p.x + _r < 0.0) return;
			if (_p.x - _r >= (float) w) return;
			if (_p.y + _r < 0.0) return;
			if (_p.y - _r >= (float)h) return;
			if (_code == CircleCode::L)
			{
				while (a < LOW_P_HPI)
				{
					x_p = sin(a) * _r;
					lxi = (int)_p.x - x_p;
					xi = (int)_p.x + x_p;
					yi = (int)(_p.y - cos(a) * _r);
					if (yi != ly)
					{
						if (ly >= 0)
						{
							mp = ly * pitch;
							xs = lx;
							xe = ox;
							if ((xe > 0) && (xs < w))
							{
								xs = xs < 0 ? 0 : xs;
								xe = xe >= w ? w - 1 : xe;
								offset = xs * 4 + mp;
								while (xs <= xe)
								{
									*((Color32*)&((uint8_t*)d)[offset]) = _col;
									offset += 4;
									xs++;
								}
							}
							xs = olx;
							xe = llx;
							if ((xe > 0) && (xs < w))
							{
								xs = xs < 0 ? 0 : xs;
								xe = xe >= w ? w - 1 : xe;
								offset = xs * 4 + mp;
								while (xs <= xe)
								{
									*((Color32*)&((uint8_t*)d)[offset]) = _col;
									offset += 4;
									xs++;
								}
							}

						}
						if (yi >= h) return;
						lx = xi;
						llx = lxi;
						ly = yi;
					}
					ox = xi;
					olx = lxi;
					oy = yi;
					a = a_d * i;
					i++;
				}
				while (a < LOW_P_PI)
				{
					x_p = sin(a) * _r;
					lxi = (int)_p.x - x_p;
					xi = (int)_p.x + x_p;
					yi = (int)(_p.y - cos(a) * _r);
					if (yi != ly)
					{
						if (ly >= 0)
						{
							mp = ly * pitch;
							xs = ox;
							xe = lx;
							if ((xe > 0) && (xs < w))
							{
								xs = xs < 0 ? 0 : xs;
								xe = xe >= w ? w - 1 : xe;
								offset = xs * 4 + mp;
								while (xs <= xe)
								{
									*((Color32*)&((uint8_t*)d)[offset]) = _col;
									offset += 4;
									xs++;
								}
							}
							xs = llx;
							xe = olx;
							if ((xe > 0) && (xs < w))
							{
								xs = xs < 0 ? 0 : xs;
								xe = xe >= w ? w - 1 : xe;
								offset = xs * 4 + mp;
								while (xs <= xe)
								{
									*((Color32*)&((uint8_t*)d)[offset]) = _col;
									offset += 4;
									xs++;
								}
							}
						}
						if (yi >= h) return;
						lx = xi;
						llx = lxi;
						ly = yi;
					}
					ox = xi;
					olx = lxi;
					oy = yi;
					a = a_d * i;
					i++;
				}
			}
			else
			{
				while (a < LOW_P_HPI)
				{
					x_p = sin(a) * _r;
					lxi = (int)_p.x - x_p;
					xi = (int)_p.x + x_p;
					yi = (int)(_p.y - cos(a) * _r);
					if (yi != ly)
					{
						if (ly >= 0)
						{
							mp = ly * pitch;
							xs = olx;
							xe = ox;
							if ((xe > 0) && (xs < w))
							{
								xs = xs < 0 ? 0 : xs;
								xe = xe >= w ? w - 1 : xe;
								offset = xs * 4 + mp;
								while (xs <= xe)
								{
									*((Color32*)&((uint8_t*)d)[offset]) = _col;
									offset += 4;
									xs++;
								}
							}
						}
						if (yi >= h) return;
						lx = xi;
						llx = lxi;
						ly = yi;
					}
					ox = xi;
					olx = lxi;
					oy = yi;
					a = a_d * i;
					i++;
				}
				while (a < LOW_P_PI)
				{
					x_p = sin(a) * _r;
					lxi = (int)_p.x - x_p;
					xi = (int)_p.x + x_p;
					yi = (int)(_p.y - cos(a) * _r);
					if (yi != ly)
					{
						if (ly >= 0)
						{
							mp = ly * pitch;
							xs = llx;
							xe = lx;
							if ((xe > 0) && (xs < w))
							{
								xs = xs < 0 ? 0 : xs;
								xe = xe >= w ? w - 1 : xe;
								offset = xs * 4 + mp;
								while (xs <= xe)
								{
									*((Color32*)&((uint8_t*)d)[offset]) = _col;
									offset += 4;
									xs++;
								}
							}
						}
						if (yi >= h) return;
						lx = xi;
						llx = lxi;
						ly = yi;
					}
					ox = xi;
					olx = lxi;
					oy = yi;
					a = a_d * i;
					i++;
				}
			}
			xs = llx;
			xe = lx;
			mp = ly * pitch;
			if ((xe > 0) && (xs < w))
			{
				xs = xs < 0 ? 0 : xs;
				xe = xe >= w ? w - 1 : xe;
				offset = xs * 4 + mp;
				while (xs <= xe)
				{
					*((Color32*)&((uint8_t*)d)[offset]) = _col;
					offset += 4;
					xs++;
				}
			}
		}
		
		void put(
			Image& _dest,
			vec2 _p,
			Image& _src,
			MethodCode _method,
			vec2 _src_s,
			vec2 _src_e,
			uint8_t _alpha)
		{
			SDL_Rect source_r;
			SDL_Rect dest_r;
			if (_dest.empty() || _src.empty())
			{
				throw std::runtime_error("No image.");
			}
			if (_method == MethodCode::NONE)
			{
				SDL_SetSurfaceBlendMode(_src.surface, SDL_BLENDMODE_NONE);
			}
			else if (_method == MethodCode::ALPHA)
			{
				SDL_SetSurfaceBlendMode(_src.surface, SDL_BLENDMODE_BLEND);
				SDL_SetSurfaceAlphaMod(_src.surface, _alpha);
			}
			else if (_method == MethodCode::ADD)
			{
				SDL_SetSurfaceBlendMode(_src.surface, SDL_BLENDMODE_ADD);
			}
			else if (_method == MethodCode::MIX)
			{
				SDL_SetSurfaceBlendMode(_src.surface, SDL_BLENDMODE_MOD);
			}
			if (_src_s.x < 0)
			{
				source_r.x = 0;
				source_r.y = 0;
				source_r.w = _src.width;
				source_r.h = _src.height;
			}
			else
			{
				source_r.x = (int) _src_e.x;
				source_r.y = (int) _src_e.y;
				source_r.w = (int) abs(_src_e.x - _src_s.x) + 1;
				source_r.h = (int) abs(_src_e.y - _src_s.y) + 1;
			}
			dest_r.x = (int)_p.x;
			dest_r.y = (int)_p.y;
			dest_r.w = source_r.w;
			dest_r.h = source_r.h;
			SDL_BlitSurface(_src.surface, &source_r, _dest.surface, &dest_r);
		}
		void putf(
			Image& _dest,
			vec2 _p,
			Image& _src,
			MethodCode _method,
			vec2 _src_s,
			vec2 _src_e,
			uint8_t _alpha)
		{
			vec2 c_tl;
			vec2 c_br;
			vec2 d_p;
			vec2 d_s;
			void* src_pixels;
			void* dst_pixels;
			int w;
			int h;
			if (_dest.empty() || _src.empty())
			{
				throw std::runtime_error("No image.");
			}
			w = _src.width;
			h = _src.height;
			if (_src_s.x < 0)
			{
				_src_s = vec2(0, 0);
				_src_e = vec2(w, h);
				d_s = vec2(w, h);
			}
			else
			{
				d_s = _src_e - _src_s;
			}
			w = _dest.width;
			h = _dest.height;
			if (!any_clip(_p, d_s, vec2(0, 0), vec2(w, h), d_p, c_tl, c_br))
			{
				return;
			}
			src_pixels = _src.get_raw_data();
			dst_pixels = _dest.data;



		}
		void draw_text(
			Image& _dest,
			vec2 _p,
			std::string _text,
			Color32 _col)
		{

		}

	}
}