#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "GLCD.h"
#include "Serial.h"
#include "CRIS_UTILS.h"



#define __FI        1                       /* Font index 16x24               */
#define crscr_msk 0x1F


volatile int exec = 1;
volatile uint32_t cursor;
volatile int x_cord0=120, y_cord0=220, x_cord1, y_cord1, fire = 0;

void delay(int n)
{
	int i;
	for(i=0;i<n;i++)        //loop to count till the delay ends
    {
    }
}



int main (void)
{

    GLCD_Init();  // (0) Initializations of GLCD
	GLCD_Clear(Black); // Clear graphical LCD display


    // configuring timer 0
    LPC_SC->PCONP |= 1 << 1; // Powering up Timer 0
    LPC_SC->PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK, i.e., CPU Clock (
    LPC_TIM0->MR0 = 1 << 18; // giving a value suitable for the LED blinking
                             // frequency based on the clock frequency
    LPC_TIM0->MCR |= 1 << 0; // Comparing Interrupt on Match 0
    LPC_TIM0->MCR |= 1 << 1; // Resetting timer on Match 0
    LPC_TIM0->TCR |= 1 << 1; // Manually Resetting Timer 0 (forced);
    LPC_TIM0->TCR &= ~(1 << 1); // Stop resetting the timer


    NVIC_EnableIRQ(TIMER0_IRQn);   // Enable timer interrupt;
		LPC_GPIOINT->IO2IntEnR |= 1 << 10;
		NVIC_EnableIRQ(EINT3_IRQn); //enabling external interrupt 3 //check forEINT3


    LPC_TIM0->TCR |= 1 << 0; // Start timer
    LPC_SC->PCONP |= ( 1 << 15 ); // Power up GPIO
    LPC_GPIO2->FIODIR |= 1 << 1; // Put P1.29 into output mode. LED is connected to P1.29

    //infinite loop
    while(1)
     {
       x_cord0 = ufo_postn();    //drawing and upgrading the position of UFO

       if(fire == 1)		//checking for interrupt when grenades are fired
					{
					 fire = 0;						//clearing interrupt
					 grenade_fire_1(x_cord0,y_cord0);		//drawing first grenade
					}
      }
}


void TIMER0_IRQHandler(void)   //interrupt handling on timer 0 if the UFO is ready for execution
	{
  if ( (LPC_TIM0->IR & 0x01) == 0x01 ) // if MR0 interrupt
    {
			LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag
			LPC_GPIO1->FIOPIN ^= 1 << 1; // toggle the P0.29 LED;
			exec = 1; //execution is allowed
    }
	}

void EINT3_IRQHandler(void) //handling the fire interrupt ( the external interrupt)
	{
  volatile int i = 0;
	LPC_GPIOINT->IO2IntClr |= 1 << 10;
 	for(i=0;i<100;i++)
		{}
	fire = 1;


	}
//functionalities of the UFO
int ufo_postn()			//for checking the movement and returning the position of the UFO
	{
	if ( exec == 1 ) 
		{ 
		plot_UFO(x_cord0,y_cord0);
				
		cursor = (LPC_GPIO1->FIOPIN >> 25) & crscr_msk;	
		LPC_GPIO2->FIOPIN = (~cursor) & crscr_msk;
		cursor=~cursor;
		if ((cursor & 0x10) && (x_cord0 < 265)) //if((cursor == 0x02) &&(x_cord0 <= 252))		//if right fire in joystick pressed
			{
			GLCD_SetTextColor(Black); //clearing the ufo from past positions
			draw_rectangle((x_cord0),(y_cord0),(x_cord0+8),(y_cord0),4);
			draw_rectangle((x_cord0+10),(y_cord0+4),(x_cord0+18),(y_cord0+4),4);
			draw_rectangle((x_cord0+10),(y_cord0-4),(x_cord0+18),(y_cord0-4),4);
			draw_rectangle((x_cord0+10),(y_cord0+8),(x_cord0+18),(y_cord0+8),4);
			draw_rectangle((x_cord0+10),(y_cord0-8),(x_cord0+18),(y_cord0-8),4);
			draw_rectangle((x_cord0+10),(y_cord0-12),(x_cord0+28),(y_cord0-12),4);
			draw_rectangle((x_cord0+10),(y_cord0+12),(x_cord0+28),(y_cord0+12),4);
				
			GLCD_SetTextColor(Yellow); 
			draw_rectangle((x_cord0+60),(y_cord0),(x_cord0+64),(y_cord0),4);
			GLCD_SetTextColor(Red);
			draw_rectangle((x_cord0+64),(y_cord0),(x_cord0+68),(y_cord0),4);
			
			GLCD_SetTextColor(Blue);
			draw_rectangle((x_cord0+50),(y_cord0+4),(x_cord0+58),(y_cord0+4),4);
			draw_rectangle((x_cord0+50),(y_cord0-4),(x_cord0+58),(y_cord0-4),4);
			draw_rectangle((x_cord0+50),(y_cord0-8),(x_cord0+58),(y_cord0-8),4);
			draw_rectangle((x_cord0+50),(y_cord0+8),(x_cord0+58),(y_cord0+8),4);
			draw_rectangle((x_cord0+40),(y_cord0+12),(x_cord0+48),(y_cord0+12),4);
			draw_rectangle((x_cord0+40),(y_cord0-12),(x_cord0+48),(y_cord0-12),4);
			//
			x_cord0 = x_cord0 + 9;		//move ufo to right by 9 positions
			//GLCD_Clear(Black);
			}	
			
			else if((cursor & (0x02)) && (x_cord0 > 9))		//left fire in joystick(cursor == 0x10) &&(x_cord0 >= 8) pressed
				{
				
			
				GLCD_SetTextColor(Black);
				draw_rectangle((x_cord0+52),(y_cord0),(x_cord0+60),(y_cord0),4);
				draw_rectangle((x_cord0+42),(y_cord0+4),(x_cord0+50),(y_cord0+4),4);
				draw_rectangle((x_cord0+42),(y_cord0-4),(x_cord0+50),(y_cord0-4),4);
				draw_rectangle((x_cord0+42),(y_cord0+8),(x_cord0+52),(y_cord0+8),4);
				draw_rectangle((x_cord0+42),(y_cord0-8),(x_cord0+52),(y_cord0-8),4);
				draw_rectangle((x_cord0+32),(y_cord0+12),(x_cord0+40),(y_cord0+12),4);
				draw_rectangle((x_cord0+32),(y_cord0-12),(x_cord0+40),(y_cord0-12),4);
			
				GLCD_SetTextColor(Yellow);
				draw_rectangle((x_cord0-4),(y_cord0),(x_cord0),(y_cord0),4);
				GLCD_SetTextColor(Red);
				draw_rectangle((x_cord0-8),(y_cord0),(x_cord0-4),(y_cord0),4);
			
				GLCD_SetTextColor(Blue);
				draw_rectangle((x_cord0+2),(y_cord0+4),(x_cord0+10),(y_cord0+4),4);
				draw_rectangle((x_cord0+2),(y_cord0+8),(x_cord0+10),(y_cord0+8),4);
				draw_rectangle((x_cord0+2),(y_cord0-8),(x_cord0+10),(y_cord0-8),4);
				draw_rectangle((x_cord0+2),(y_cord0-4),(x_cord0+10),(y_cord0-4),4);
				draw_rectangle((x_cord0+12),(y_cord0+12),(x_cord0+20),(y_cord0+12),4);
				draw_rectangle((x_cord0+12),(y_cord0-12),(x_cord0+20),(y_cord0-12),4);
			
				x_cord0 = x_cord0 - 9;		//moove ufo to left by 9 positions
				//GLCD_Clear(Black);
				}
			
			else
				x_cord0 = x_cord0;
			exec = 0;
		}
	return(x_cord0);
	}

int grenade_fire_1(int x_cord1, int y_cord1)		//this function is used to draw 1 grenade at a time
	{
	  int y1=y_cord1,x1=x_cord1;
		//NVIC_ClearPendingIRQ(EINT3_IRQn);
		while(y1>12)
		{
		x_cord0 = ufo_postn();			//update ufo is joystick fire is pressed
		GLCD_SetTextColor(Purple);
		if(y1>12)
			plot_grenade(x1,y1);	
		if(fire == 1)		//check for interrupt
			{
			GLCD_Clear(Black);
			fire = 0;	//clear interrupt
			//NVIC_ClearPendingIRQ(EINT3_IRQn);
			grenade_fire_2(x_cord0,y_cord0,x1,y1);	//draw 2 grenades
			break;
			}
		delay(3000000);
		GLCD_SetTextColor(Black);
		plot_grenade(x1,y1);
		y1=y1-12;	
			
		}
	
	return(0);
	}


int grenade_fire_2(int x_cord1, int y_cord1, int x1, int y1)		//this function draws 2 grenades
	{
	 int y2=y_cord1,x2=x_cord1;
	 //NVIC_ClearPendingIRQ(EINT3_IRQn);		//clear any pending interrupts
	 fire = 0;
	
	while(y2>12 || y1>12)		//move until 2nd grenade reaches top
		{	
		x_cord0 = ufo_postn();		//update ufo
		GLCD_SetTextColor(Purple);
		if(y2>12)
			plot_grenade(x2,y2);	
		plot_grenade(x1,y1);
		
		delay(3000000);
		GLCD_SetTextColor(Black);
		if(y2>12)
			plot_grenade(x2,y2);
		plot_grenade(x1,y1);
		y2=y2-12;
		y1=y1-12;
		if(y1<8)		//if 1st grenade reaches top, then go to grenade 1
			{
			GLCD_Clear(Black);
			NVIC_ClearPendingIRQ(EINT3_IRQn);
			grenade_fire_1(x1,y1+8);
			break;
			}
      
		if(fire == 1)		//check for interrupt
			{
			fire = 0;		//clear interrupt
			GLCD_Clear(Black);
			NVIC_ClearPendingIRQ(EINT3_IRQn);
			grenade_fire_3(x_cord0,y_cord0,x1,y1,x2,y2);		//draws 3 grenade
			break;
			}
		}
		return(0);
	}



int grenade_fire_3(int x_cord1, int y_cord1, int x1, int y1, int x2, int y2)
	{	
		int y3=y_cord1,x3=x_cord1;
		NVIC_ClearPendingIRQ(EINT3_IRQn);
	while(y3>12 || y2>12 || y1>12)		//draw until 3rd grenade reaches top
		{
		x_cord0 = ufo_postn();		//update ufo
		GLCD_SetTextColor(Purple);
		plot_grenade(x3,y3);
		plot_grenade(x2,y2);
		plot_grenade(x1,y1);
		NVIC_DisableIRQ(EINT0_IRQn);			//disable interrupt by not allowing any more greandes
		fire=0;
		delay(3000000);
		GLCD_SetTextColor(Black);
		plot_grenade(x3,y3);
		plot_grenade(x2,y2);
		plot_grenade(x1,y1);
		y3=y3-12;
		y2=y2-12;
		y1=y1-12;
		if(y1<8)				//if 1st grenade reaches top, go to grenade 2
			{
			GLCD_Clear(Black);
			NVIC_ClearPendingIRQ(EINT3_IRQn);
			NVIC_EnableIRQ(EINT3_IRQn);		//enable interrupt
			grenade_fire_2(x2,y2-8,x3,y3+2);
			break;
			}
		else if (y2<8)
		{
		  GLCD_Clear(Black);
			NVIC_EnableIRQ(EINT3_IRQn);		//enable interrupt
			grenade_fire_1(x3,y3-5);
			break;
		}
			
		}
	return(0);
	}	
