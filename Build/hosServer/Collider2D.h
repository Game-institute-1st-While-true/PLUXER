#pragma once
#if __has_include("DXTK\SimpleMath.h")
#include "DXTK\SimpleMath.h"
#ifndef DXTK_MATH
#define DXTK_MATH
#endif // !DXTK_MATH
#endif // __has_include("DXTK\SimpleMath.h")


namespace collider2
{
		struct Rect
		{
			DirectX::SimpleMath::Vector2 Top;
			DirectX::SimpleMath::Vector2 Bottom;

		public:
			Rect()
			{
				Top.x = -0.5;
				Top.y = 0.5;

				Bottom.x = 0.5;
				Bottom.y = -0.5;
			}

			Rect(const DirectX::SimpleMath::Vector2& top, const DirectX::SimpleMath::Vector2& bottom)
			{
				Top = top;
				Bottom = bottom;
			}

			Rect(const DirectX::SimpleMath::Vector2& center, const float radius)
			{
				Top.x = center.x - radius;
				Top.y = center.y + radius;

				Bottom.x = center.x + radius;
				Bottom.y = center.y - radius;
			}

			void Set(const DirectX::SimpleMath::Vector2& top, const DirectX::SimpleMath::Vector2& bottom)
			{
				Top = top;
				Bottom = bottom;
			}

			void Set(const DirectX::SimpleMath::Vector2& center, const float length)
			{
				Top.x = center.x - length;
				Top.y = center.y + length;

				Bottom.x = center.x + length;
				Bottom.y = center.y - length;
			}

			void SetSizeCenter(const DirectX::SimpleMath::Vector2& size, const DirectX::SimpleMath::Vector2& center)
			{
				DirectX::SimpleMath::Vector2 offset = size / 2;

				Top.x = center.x - offset.x;
				Top.y = center.y + offset.y;

				Bottom.x = center.x + offset.x;
				Bottom.y = center.y - offset.y;
			}		
		};

		struct Circle
		{
			DirectX::SimpleMath::Vector2 Center;
			double Radius;
		};

		struct Line
		{
			DirectX::SimpleMath::Vector2 Src;
			DirectX::SimpleMath::Vector2 Dst;
		};

		static bool AABB(const Rect& a, const Rect& b)
		{
			if (a.Top.x < b.Bottom.x && a.Bottom.x > b.Top.x &&
				a.Top.y > b.Bottom.y && a.Bottom.y < b.Top.y)
			{
				return true;
			}
			return false;
		}

		static bool OBB(const Rect& a, const Rect& b)
		{
			DirectX::SimpleMath::Vector2 rt1[4]; // lt 0 , lb 1 , rb 2 , rt 3

			rt1[0].x = a.Top.x; 
			rt1[0].y = a.Top.y;	

			rt1[1].x = a.Top.x;
			rt1[1].y = a.Bottom.y;

			rt1[2].x = a.Bottom.x;
			rt1[2].y = a.Bottom.y;

			rt1[3].x = a.Bottom.x;
			rt1[3].y = a.Top.y;
			
			DirectX::SimpleMath::Vector2 rt2[4];

			rt2[0].x = b.Top.x;
			rt2[0].y = b.Top.y;

			rt2[1].x = b.Top.x;
			rt2[1].y = b.Bottom.y;

			rt2[2].x = b.Bottom.x;
			rt2[2].y = b.Bottom.y;

			rt2[3].x = b.Bottom.x;
			rt2[3].y = b.Top.y;

			DirectX::SimpleMath::Vector2 vec;

			float p1[2] = { 0.0f, };
			float p2[4] = { 0.0f, };

			float rect1Min;
			float rect1Max;
			float rect2Min;
			float rect2Max;

			vec = rt1[0] - rt1[1];

			p1[0] = vec.Dot(rt1[0]);
			p1[1] = vec.Dot(rt1[1]);

			p2[0] = vec.Dot(rt2[0]);
			p2[1] = vec.Dot(rt2[1]);
			p2[2] = vec.Dot(rt2[2]);
			p2[3] = vec.Dot(rt2[3]);

			rect1Min = fmin(p1[0], p1[1]);
			rect1Max = fmax(p1[0], p1[1]);

			rect2Min = fmin(fmin(fmin(p2[0], p2[1]), p2[2]), p2[3]);
			rect2Max = fmax(fmax(fmax(p2[0], p2[1]), p2[2]), p2[3]);
	
			if (!(rect1Min < rect2Max && rect2Min < rect1Max))
			{
				return false;
			}

			vec = rt1[1] - rt1[2];

			p1[0] = vec.Dot(rt1[1]);
			p1[1] = vec.Dot(rt1[2]);

			p2[0] = vec.Dot(rt2[0]);
			p2[1] = vec.Dot(rt2[1]);
			p2[2] = vec.Dot(rt2[2]);
			p2[3] = vec.Dot(rt2[3]);

			rect1Min = fmin(p1[0], p1[1]);
			rect1Max = fmax(p1[0], p1[1]);

			rect2Min = fmin(fmin(fmin(p2[0], p2[1]), p2[2]), p2[3]);
			rect2Max = fmax(fmax(fmax(p2[0], p2[1]), p2[2]), p2[3]);

			if (!(rect1Min < rect2Max && rect2Min < rect1Max))
			{
				return false;
			}

			vec = rt2[0] - rt2[1];

			p1[0] = vec.Dot(rt2[0]);
			p1[1] = vec.Dot(rt2[1]);

			p2[0] = vec.Dot(rt1[0]);
			p2[1] = vec.Dot(rt1[1]);
			p2[2] = vec.Dot(rt1[2]);
			p2[3] = vec.Dot(rt1[3]);

			rect1Min = fmin(p1[0], p1[1]);
			rect1Max = fmax(p1[0], p1[1]);

			rect2Min = fmin(fmin(fmin(p2[0], p2[1]), p2[2]), p2[3]);
			rect2Max = fmax(fmax(fmax(p2[0], p2[1]), p2[2]), p2[3]);

			if (!(rect1Min < rect2Max && rect2Min < rect1Max))
			{
				return false;
			}

			vec = rt2[1] - rt2[2];

			p1[0] = vec.Dot(rt2[1]);
			p1[1] = vec.Dot(rt2[2]);

			p2[0] = vec.Dot(rt1[0]);
			p2[1] = vec.Dot(rt1[1]);
			p2[2] = vec.Dot(rt1[2]);
			p2[3] = vec.Dot(rt1[3]);

			rect1Min = fmin(p1[0], p1[1]);
			rect1Max = fmax(p1[0], p1[1]);

			rect2Min = fmin(fmin(fmin(p2[0], p2[1]), p2[2]), p2[3]);
			rect2Max = fmax(fmax(fmax(p2[0], p2[1]), p2[2]), p2[3]);

			if (!(rect1Min < rect2Max && rect2Min < rect1Max))
			{
				return false;
			}
			return true;
		};

		static bool LC(const Line& line, const Circle& circle)
		{
			DirectX::SimpleMath::Vector2 src_dst;
			src_dst = line.Src - line.Dst;
			src_dst.Normalize();

			DirectX::SimpleMath::Vector2 src_center;
			src_center = line.Src - circle.Center;
			src_center.Normalize();

			float cos_theta = src_dst.Dot(src_center);

			float src_center_len = src_center.Length();
			float projection = src_center.Length() * cos_theta;
			
			float distance = (src_center_len * src_center_len) - (projection * projection);
			distance = sqrt(distance);

			if (distance > circle.Radius)
			{
				false;
			}
			if (distance <= circle.Radius)
			{
				true;
			}
			return false;
		};

		static bool CC(const Circle& c1, const Circle& c2)
		{
			DirectX::SimpleMath::Vector2 v;

			v.x = c1.Center.x - c2.Center.x;
			v.y = c1.Center.y - c2.Center.y;

			float distance = sqrtf(v.x * v.x + v.y * v.y);

			if (distance > (c1.Radius + c2.Radius))
			{
				return false;
			}
			if (distance <= (c1.Radius + c2.Radius))
			{
				return true;
			}

			return false;
		};

		static bool RECT_POINT(const Rect& rt, DirectX::SimpleMath::Vector2& pt)
		{
			if (rt.Top.x <= pt.x && pt.x <= rt.Bottom.x 
				&& rt.Top.y >= pt.y && pt.y >= rt.Bottom.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		};

		static bool CIRCLE_POINT(const Circle& c, DirectX::SimpleMath::Vector2& pt)
		{
			DirectX::SimpleMath::Vector2 v;

			v.x = c.Center.x - pt.x;
			v.y = c.Center.y - pt.y;

			float distance = sqrtf(v.x * v.x + v.y * v.y);

			if (distance > c.Radius)
			{
				return false;
			}
			if (distance <= c.Radius)
			{
				return true;
			}

		}
};