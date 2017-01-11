// Cristinel Ababei, 2013
// this is a collection of various "graphics" functions used for
// drawing stuff on the LCD screen of the MCB1700 development board;
// the vast majority of these are put together with a lot
// of open source code, therefore with minimal effort;

#include <stdio.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "GLCD.h"
#include "CRIS_UTILS.h"

#define __FI        1                       /* Font index 16x24               */

volatile unsigned short lcd_colors[] = { // 16 colors
        Red,Yellow};
 volatile unsigned short last_color = Yellow,last_color1,last_color2;
    volatile int last_x0=0, last_y0=0;
    volatile int i=0, last_r=1;                                                                       

// this is simply the midpoint circle algorithm for drawing a circle;
// it is a variant of Bresenham's line algorithm, and is thus sometimes 
// known as Bresenham's circle algorithm;
// see additional info:
// http://en.wikipedia.org/wiki/Midpoint_circle_algorithm  
// http://www.mathsisfun.com/algebra/circle-equations.html
// http://members.chello.at/~easyfilter/bresenham.html
void CRIS_draw_circle( int x0, int y0, int radius)
{
    int x = radius, y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = radius;
	  


    while (x >= y) 
    {
        GLCD_PutPixel(x + x0, y + y0); // call of GCLD function;
        GLCD_PutPixel(y + x0, x + y0);
        GLCD_PutPixel(-x + x0, y + y0);
        GLCD_PutPixel(-y + x0, x + y0);
        GLCD_PutPixel(-x + x0, -y + y0);
        GLCD_PutPixel(-y + x0, -x + y0);
        GLCD_PutPixel(x + x0, -y + y0);
        GLCD_PutPixel(y + x0, -x + y0);

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

// this is similar to CRIS_draw_circle() but the circle is also filled;
void CRIS_draw_circle_half( int x0, int y0, int radius) 
{
    int x = radius, y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y) 
    {
        //GLCD_PutPixel(x + x0, y + y0); // call of GCLD function;
        GLCD_PutPixel(y + x0, x + y0);
        //GLCD_PutPixel(-x + x0, y + y0);
        GLCD_PutPixel(-y + x0, x + y0);
        /*GLCD_PutPixel(-x + x0, -y + y0);
        GLCD_PutPixel(-y + x0, -x + y0);
        GLCD_PutPixel(x + x0, -y + y0);
        GLCD_PutPixel(y + x0, -x + y0);*/

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

// this is the so called Bresenham's line algorithm;
// draws a line between two points specified thru their
// x,y coordinates;
// see additional info:
// http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
// TODO: http://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
void CRIS_draw_line( int x0, int y0, int x1, int y1) 
{ 
    int dx = x0<x1 ? (x1-x0) : (x0-x1); // basically dx=abs(x1-x0) 
    int sx = x0<x1 ? 1 : -1;
    int dy = y0<y1 ? (y1-y0) : (y0-y1);
    int sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;

    for (;;) {
        GLCD_PutPixel(x0,y0); // call of GCLD function;
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

// this draws a simplified smiley face;
// what would you change to improve it?
void CRIS_draw_smiley_face( int x0, int y0, int radius)
{
	while(1)
	{
		
	for(i=0;i<=radius;i++)
			{
				last_x0 = x0;//rand() % 320 + 1; // last_x0 in the range 1-320
        last_y0 = y0;//rand() % 240 + 1; // last_r in the range 1-240
        last_r = i;//rand() % 16 + 4; // last_r in the range 4-20
        //last_color = Red;//lcd_colors[ 7 ];
        GLCD_SetTextColor(last_color);
        CRIS_draw_circle(last_x0, last_y0, last_r);
		}
		for(i=0;i<=15;i++)
			{
				last_x0 = x0+30;//rand() % 320 + 1; // last_x0 in the range 1-320//190
        last_y0 = y0-30;//rand() % 240 + 1; // last_r in the range 1-240//90
        last_r = i;//rand() % 16 + 4; // last_r in the range 4-20
        last_color1 = Black;//lcd_colors[ 7 ];
        GLCD_SetTextColor(last_color1);
        CRIS_draw_circle(last_x0, last_y0, last_r);
			}	
		for(i=0;i<=15;i++)
			{
				last_x0 = x0-30;//rand() % 320 + 1; // last_x0 in the range 1-320//130
        last_y0 = y0-30;//rand() % 240 + 1; // last_r in the range 1-240//90
        last_r = i;//rand() % 16 + 4; // last_r in the range 4-20
        last_color1 = Black;//lcd_colors[ 7 ];
        GLCD_SetTextColor(last_color1);
        CRIS_draw_circle(last_x0, last_y0, last_r);
			}
			for(i=48;i<=50;i++)
			{
				last_x0 = x0;//rand() % 320 + 1; // last_x0 in the range 1-320
        last_y0 = y0;//rand() % 240 + 1; // last_r in the range 1-240
        last_r = i;//rand() % 16 + 4; // last_r in the range 4-20
        last_color2 = Black;//lcd_colors[ 7 ];
        GLCD_SetTextColor(last_color2);
        CRIS_draw_circle_half(last_x0, last_y0, last_r);
			}
			for(i=0;i<1000000;i++)
			{}
				if(last_color==Red)
				last_color=Yellow;
			else
				last_color=Red;
			
			
				
		}
    
}
void draw_line( int x0, int y0, int x1, int y1) 
{ 
    int dx = x0<x1 ? (x1-x0) : (x0-x1); // basically dx=abs(x1-x0) 
    int sx = x0<x1 ? 1 : -1;
    int dy = y0<y1 ? (y1-y0) : (y0-y1);
    int sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;

    for (;;) {
        GLCD_PutPixel(x0,y0); // call of GCLD function;
        if (x0==x1 && y0==y1) break;																																																																																											
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

int draw_rectangle(int x_ufo_1, int y_ufo_1, int x2, int y2, int n)
	{
	int i;
	for(i = 0; i<n; i++)		//draws a rectangle
		{
		draw_line(x_ufo_1,(y_ufo_1+i),x2,(y_ufo_1+i));   
		}
	return(0);
	}
int plot_UFO(int x_ufo_0, int y_ufo_0)		//plotting ufo
	{
	volatile int i;		
	for(i=0;i<15;i++)		//used to draw the centre rectangle of ufo with alternating red and yellow color
			{
			last_color = Red;
			last_color = lcd_colors[ i % 2 ];	
			GLCD_SetTextColor(last_color);
			draw_rectangle((x_ufo_0+(4*i)),y_ufo_0,(x_ufo_0+(4*i)+4),y_ufo_0,4);	
			}
			
		GLCD_SetTextColor(Blue);	
		draw_rectangle((x_ufo_0+10),(y_ufo_0+4),(x_ufo_0+50),(y_ufo_0+4),4);
		draw_rectangle((x_ufo_0+10),(y_ufo_0+8),(x_ufo_0+50),(y_ufo_0+8),4);
		draw_rectangle((x_ufo_0+10),(y_ufo_0-4),(x_ufo_0+50),(y_ufo_0-4),4);
		draw_rectangle((x_ufo_0+10),(y_ufo_0-8),(x_ufo_0+50),(y_ufo_0-8),4);
		draw_rectangle((x_ufo_0+20),(y_ufo_0+12),(x_ufo_0+40),(y_ufo_0+12),4);
		draw_rectangle((x_ufo_0+20),(y_ufo_0-12),(x_ufo_0+40),(y_ufo_0-12),4);
		return(0);
	}
	


int plot_grenade(int x_ufo_1, int y_ufo_1)		//plotting grenade
	{
	draw_rectangle((x_ufo_1+29),(y_ufo_1-16),(x_ufo_1+33),(y_ufo_1-16),4);
	draw_rectangle((x_ufo_1+29),(y_ufo_1-20),(x_ufo_1+33),(y_ufo_1-20),4);
	draw_rectangle((x_ufo_1+30),(y_ufo_1-23),(x_ufo_1+32),(y_ufo_1-23),4);
	
	return(0);
	}
