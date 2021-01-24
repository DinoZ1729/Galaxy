#include <iostream>
#include "screen.h"
#include "vec2.h"
struct Body
{
	float r;
	float m;
	vec2 pos;
	vec2 vel;
	vec2 acc;
	
	Body()
	{
		m=1.0f;
		r=0.2f;
		pos=0;
		vel=0;
		acc=0;
	}
	
	Body(float m)
	:m(m)
	{
		r=0.2f*cbrt(m);
		pos=0;
		vel=0;
		acc=0;
	}
	
	Body(float m,float r) : r(r), m(m)
	{
		pos=0;
		vel=0;
		acc=0;
	}
	
	void setPos(float x,float y)
	{
		pos.x=x;
		pos.y=y;
	}
	
	void PulledBy(const Body& other)
	{
		const float G=1.0f;
		float dist = sqrt( (pos-other.pos)*(pos-other.pos) );
		acc += G*other.m*(other.pos-pos)/dist/dist/dist;
	}
	
	void Update(float dt)
	{
		vel+=dt*acc;
		pos+=dt*vel;
		acc=0;
	}
	
	
};

void Plot(const Body& body, Screen& scr)
{
	scr.PlotCircle(body.pos.x,body.pos.y,body.r);
}

int main()
{
	Screen scr(0,0,10);
	
	constexpr float dt=1.0/100.0f;
	constexpr float r=5.5f,R=30.0f;
	
	Body Sun(10000.0f,7.0f);
	Body Earth(1000.0f,2.0f);
	Body Moon(1.0f,1.2f);
	
	Sun.pos={0.0f,0.0f};
	Sun.vel={0.0f,0.0f};
	
	Earth.pos={R,0.0f};
	Earth.vel={0.0f,sqrt(Sun.m/R)};
	
	Moon.pos={R,r};
	Moon.vel={sqrt(Earth.m/r),Earth.vel.y};
	
	while(true)
	{
		scr.Clear();
		
		Moon.PulledBy(Earth);
		Moon.PulledBy(Sun);
		Earth.PulledBy(Moon);
		Earth.PulledBy(Sun);
		Sun.PulledBy(Moon);
		Sun.PulledBy(Earth);
		
		Moon.Update(dt);
		Earth.Update(dt);
		Sun.Update(dt);
		
		scr.Position(Sun.pos.x,Sun.pos.y);
		
		Plot(Moon,scr);
		Plot(Earth,scr);
		Plot(Sun,scr);
		
		scr.Draw();
	}
	return 0;
}
