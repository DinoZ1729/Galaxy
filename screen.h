#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

float random(float LO, float HI){
	return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

struct ball{
	float x,y,z,r,m;
	float dx, dy, dz;
};

constexpr int n = 500;
constexpr float gamma = 1;

ball balls[n];

void init(){
	for(int i=0;i<n;i++){
		balls[i].dx = random(-1,1);
		balls[i].dy = random(-1,1);
		balls[i].dz = random(-1,1);
		balls[i].x = random(-1,1);
		balls[i].y = random(-1,1);
		balls[i].z = 3 + random(-1,1);
		balls[i].r = random(0.1,1);
		balls[i].m = random(0.1,5);
	}
}

void update_vel(float dt){
	for(int i=0;i<n;i++)
	for(int j=i+1;j<n;j++){
		float r = sqrt(
		(balls[i].x-balls[j].x)*(balls[i].x-balls[j].x)+
		(balls[i].y-balls[j].y)*(balls[i].y-balls[j].y)+
		(balls[i].z-balls[j].z)*(balls[i].z-balls[j].z)
		);
		float f = gamma*balls[i].m*balls[j].m/r/r;
		balls[i].dx+=dt*(balls[j].x-balls[i].x)*f/r;
		balls[i].dy+=dt*(balls[j].y-balls[i].y)*f/r;
		balls[i].dz+=dt*(balls[j].z-balls[i].z)*f/r;
		balls[j].dx-=dt*(balls[j].x-balls[i].x)*f/r;
		balls[j].dy-=dt*(balls[j].y-balls[i].y)*f/r;
		balls[j].dz-=dt*(balls[j].z-balls[i].z)*f/r;
	}
}
void update_pos(float dt){
	for(int i=0;i<n;i++){
		balls[i].x+=dt*balls[i].dx;
		balls[i].y+=dt*balls[i].dy;
		balls[i].z+=dt*balls[i].dz;
	}
}

constexpr int nc = 6;
constexpr char ch[nc] = {' ','.',',','o','O','@'};

int clamp(int x, int mi, int ma){
	if(x<mi)
	    return mi;
	if(x>ma)
	    return ma;
	return x;
}

void nacrtaj(){
	system("cls");
	constexpr int H = 50;
	int platno[H][H];
	for(int i=0;i<H;i++)
	for(int j=0;j<H;j++)
		platno[i][j] = 0;
	
	for(int i=0;i<n;i++){
		float x = balls[i].x/balls[i].z;
		float y = balls[i].y/balls[i].z;
		int X = x*H/2 + H/2, Y = y*H/2 + H/2;
		if(0<=X&&X<H&&0<=Y&&Y<H){
			platno[X][Y]++;
		}
	}
	char pplatno[H][2*H+1];
	for(int i=0;i<H-1;i++)
		pplatno[i][2*H]='\n';
	pplatno[H-1][2*H]='\0';
	
	for(int i=0;i<H;i++)
	for(int j=0;j<H;j++){
		char c = ch[clamp(platno[i][j],0,nc-1)];
		pplatno[i][2*j] = pplatno[i][2*j+1] = '@';
	}
	
	puts(pplatno[0]);
}

int main(){
	srand (static_cast <unsigned> (time(0)));
	void init();
	constexpr float dt = 0.001;
	while(true){
		nacrtaj();
		//update_vel(dt);
		//update_pos(dt);
	}
	return 0;
}
