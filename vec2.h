#include <cmath>

struct vec2
{
	float x, y;
	
	vec2()
	{
		x=0.0f;
		y=0.0f;
	}
	
	vec2(float x, float y)
	:x(x),y(y)
	{}
	
	vec2(float k)
	{
		x=k;
		y=k;
	}
	
	vec2 operator + (const vec2& v) const
	{
		return vec2
		(
			x + v.x,
			y + v.y
		);
	}
	
	vec2 operator - (const vec2& v) const
	{
		return vec2
		(
			x - v.x,
			y - v.y
		);
	}
	
	float operator * (const vec2& v) const
	{
		return x*v.x + y*v.y;
	}
	
	vec2 operator * (float k) const
	{
		return vec2
		(
			k*x,
			k*y
		);
	}
	vec2 operator / (float k) const
	{
		return vec2
		(
			x/k,
			y/k
		);
	}
	
	void operator += (const vec2& v)
	{
		x += v.x;
		y += v.y;
	}
	
	void operator -= (const vec2& v)
	{
		x -= v.x;
		y -= v.y;
	}
	
	void operator *= (float k)
	{
		x *= k;
		y *= k;
	}
	
	void operator /= (float k)
	{
		x /= k;
		y /= k;
	}
	
	vec2 operator - ()
	{
		return vec2(-x,-y);
	}
	
	float magnitute2() const
	{
		return x*x + y*y;
	}
	
	float magnitute() const
	{
		return sqrt(magnitute2());
	}
	
	void normalize()
	{
		float mag=magnitute();
		if(mag==0.0f)
			return;
		(*this)/=mag;
	}
};

vec2 operator * (float k, const vec2& v)
{
	return v*k;
}
