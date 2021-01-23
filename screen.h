#pragma once
#include <iostream>
#include <windows.h>
#include <string>

constexpr int WIDTH = 950, HEIGHT = 670;
constexpr int dW = 8, dH = 8;

void gotoxy(short x, short y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

void setup()
{
	CONSOLE_FONT_INFOEX cf = { 0 };
	cf.cbSize = sizeof cf;
	cf.dwFontSize.X = dW;
	cf.dwFontSize.Y = dH;
	wcscpy(cf.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),0,&cf);

	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, 0, 0, 1024, 768, TRUE);
}

class Screen
{
public:
	Screen()
	{
		x=y=0;
		zoom=1;
		Clear();
	}
	
	Screen(float x, float y, float zoom=1)
		:x(x),y(y),zoom(zoom)
	{
		Clear();
	}
	void Clear()
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				canvas[i][j] = false;
			}
		}
	}
	
	void PlotPoint(float x, float y)
	{
		int pos[2];
		transform(pos, x, y);
		drawPoint(pos[0], pos[1]);
	}

	void PlotLine(float x1, float y1, float x2, float y2)
	{
		int pos1[2], pos2[2];
		transform(pos1, x1, y1);
		transform(pos2, x2, y2);
		drawLine(pos1[0], pos1[1], pos2[0], pos2[1]);
	}

	void PlotCircle(float x, float y, float r)
	{
		int p1[2], p2[2],p[2];
		transform(p1, x - r, y + r);
		transform(p2, x + r, y - r);

		for (int i = p1[0]; i <= p2[0]; i++)
		{
			for (int j = p1[1]; j <= p2[1]; j++)
			{
				float
					xt = (float)(j - WIDTH / 2) / zoom + this->x,
					yt = (float)(HEIGHT / 2 - 1 - i) / zoom + this->y;
				float radius2 = (xt-x)*(xt-x)+(yt-y)*(yt-y);
				if (radius2 <=r*r)
					drawPoint(i, j);
			}
		}
	}
	
	void PlotRectangle(float x1, float y1, float x2, float y2)
	{
		int p1[2], p2[2];
		transform(p1, x1, y1);
		transform(p2, x2, y2);
		drawRectangle(p1[0],p1[1],p2[0],p2[1]);
	}
	
	void Position(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void Zoom(float zoom)
	{
		this->zoom = zoom;
	}

	void Draw()
	{
		gotoxy(0, 0);
		char frame[HEIGHT / dH][WIDTH / dW + 1];
		for(int i=0;i<HEIGHT / dH - 1;i++)
			frame[i][WIDTH / dW]='\n';
		frame[HEIGHT / dH - 1][WIDTH/dW]='\0';
		
		for (int i = 0; i < HEIGHT / dH; i++)
		{
			for (int j = 0; j < WIDTH / dW; j++)
			{
				int count = 0;
				
				//calculating brightness
				for (int k = 0; k < dH; k++)
				{
					for (int l = 0; l < dW; l++)
					{
						count+= canvas[dH * i + k][dW * j + l];
					}
				}
				frame[i][j]=brightness(count);
			}
		}
		//borders
		for(int i=0;i<HEIGHT / dH;i++)
		{
			frame[i][0]='@';
			frame[i][WIDTH / dW - 1]='@';
		}
		for(int j=0;j<WIDTH / dW;j++)
		{
			frame[0][j]='@';
			frame[HEIGHT / dH - 1][j]='@';
		}
		puts(frame[0]);
	}
	
	int Height()
	{
		return HEIGHT;
	}

	int Width()
	{
		return HEIGHT;
	}
	
	void set_palette(int _palette)
	{
		this->_palette=_palette;
	}

private:
	bool canvas[HEIGHT][WIDTH];
	float x, y;
	float zoom;

	void transform(int pos[2],float x, float y)
	{
		//from world to screen coordinates
		x -= this->x;
		y -= this->y;

		x *= zoom;
		y *= zoom;

		x += WIDTH / 2;
		y += HEIGHT / 2;

		pos[0] = (int)(HEIGHT - 1 - y);
		pos[1] = (int)x;
	}

	//drawing functions

	void drawPoint(int A, int B)
	{
		if (A < 0 || B < 0 || A >= HEIGHT || B >= WIDTH)	return;
		canvas[A][B] = true;
	}
	
	void drawBoldPoint(int A, int B)
	{
		for(int i=A-1;i<=A+1;i++){
		for(int j=B-1;j<=B+1;j++){
			drawPoint(i,j);
		}}
	}

	void drawLine(int A, int B, int C, int D)
	{
		//sorting
		if (A > C)
		{
			int t;
			t = A;
			A = C;
			C = t;
			t = B;
			B = D;
			D = t;
		}
		//algorithm
		if (B == D)
		{
			for (int i = A; i <= C; i++)
				drawBoldPoint(i, B);
			return;
		}
		if (A == C)
		{
			int min = B, max = D;
			if (D < B)
			{
				min = D;
				max = B;
			}
			for (int i = min; i <= max; i++)
				drawBoldPoint(A, i);
			return;
		}
		if (abs(D - B) < abs(C - A))
			drawLineLow(A, B, C, D);
		else
		{
			if (B > D)	drawLineHigh(C, D, A, B);
			else	drawLineHigh(A, B, C, D);
		}
	}
	
	void drawRectangle(int i1, int j1, int i2, int j2)
	{
		int minI=i1<i2?i1:i2;
		int maxI=i1<i2?i2:i1;
		int minJ=j1<j2?j1:j2;
		int maxJ=j1<j2?j2:j1;
		for (int i = minI; i <= maxI; i++){
		for (int j = minJ; j <= maxJ; j++){
			drawPoint(i, j);
		}}
	}
	
	void drawLineLow(int x0, int y0, int x1, int y1)
	{
		int dx = x1 - x0, dy = y1 - y0, yi = 1;
		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}
		int D = 2 * dy - dx;
		int y = y0;

		for (int x = x0; x <= x1; x++)
		{
			drawBoldPoint(x, y);
			if (D > 0)
			{
				y += yi;
				D -= 2 * dx;
			}
			D += 2 * dy;
		}
	}

	void drawLineHigh(int x0, int y0, int x1, int y1)
	{
		int dx = x1 - x0, dy = y1 - y0, xi = 1;
		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}
		int D = 2 * dx - dy;
		int x = x0;

		for (int y = y0; y <= y1; y++)
		{
			drawBoldPoint(x, y);
			if (D > 0)
			{
				x += xi;
				D -= 2 * dy;
			}
			D += 2 * dx;
		}
	}
	
	int _palette=0;
	
	char brightness(int count)
	{
		std::string palette;
		switch(_palette)
		{
			case 0:
				palette = " .,:;oOQ#@";
			break;
			case 1:
				palette = "     .oO@@";
			break;
			case 2:
				palette = " .:";
			break;
			default:
				palette = " ";
			break;
		}
		return palette[count * (palette.length() - 1) / dW / dH];
	}
};
