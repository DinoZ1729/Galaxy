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
	vec2 O=body.pos;
	vec2 X=body.pos + 0.5f*body.vel;
	
	scr.PlotCircle(body.pos.x,body.pos.y,body.r);
	scr.PlotLine
	(
		O.x,
		O.y,
		X.x,
		X.y
	);
	
	vec2 a=(O-X);
	a.normalize();
	a*=0.1f;
	vec2 b={a.y,-a.x};
	
	scr.PlotLine
	(
		X.x,
		X.y,
		X.x + a.x + b.x,
		X.y + a.y + b.y
	);
	
	scr.PlotLine
	(
		X.x,
		X.y,
		X.x + a.x - b.x,
		X.y + a.y - b.y
	);
}

int main()
{
	Screen scr(0,0,200);
	
	constexpr float dt=1.0/100.0f;

	const int n=3;
	Body solarSystem[n];
	
	solarSystem[0].pos = {-0.97000436f, 0.24308753f};
	solarSystem[0].vel = {0.4662036850f, 0.4323657300f};
	solarSystem[0].m = 1.0f;
	solarSystem[0].r = 0.1f;
	
	solarSystem[1].pos = {0.0f, 0.0f};
	solarSystem[1].vel = {-0.93240737f, -0.86473146f};
	solarSystem[1].m = 1.0f;
	solarSystem[1].r = 0.1f;
	
	solarSystem[2].pos = {0.97000436f, -0.24308753f};
	solarSystem[2].vel = {0.4662036850f, 0.4323657300f};
	solarSystem[2].m = 1.0f;
	solarSystem[2].r = 0.1f;
	
	while(true)
	{
		scr.Clear();
		for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(i==j)
				continue;
			solarSystem[j].PulledBy(solarSystem[i]);
		}}
		
		for(int i=0;i<n;i++)
			solarSystem[i].Update(dt);
		for(int i=0;i<n;i++)
		{
			Plot(solarSystem[i],scr);
		}
		
		scr.Draw();
		for(auto body:solarSystem)
			printf("%+16.8f %+16.8f\n",body.vel.x,body.vel.y);
		
	}

	return 0;
}
