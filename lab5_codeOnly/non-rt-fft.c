/*
 * non-rt-fft.c
 *
 *  Created on: Mar 24, 2016
 *      Author: villarrealh
 */

#include "dsk6713_aic23.h"
#include "math.h"
Uint32 samprate = DSK6713_AIC23_FREQ_16KHZ;

extern far void vectors();  //  this is needed to find the vecs.asm file for int processing
void init_DSK(void);
void init_Mcbsp1(void);
void init_Ints(void);

union {
	Uint32 two;
	short lr[2];
}  AIC_data;


#define sampling_rate 16000.
#define freq_left 1003.
#define freq_right 2000.
#define scale 10000.0
#define pi 3.141592653589

/* phase increment left for sine wave */
float delta_left = 2.0*pi*freq_left/sampling_rate;
/* phase increment for right sine wave */
float delta_right = 2.0*pi*freq_right/sampling_rate;

float thetal = pi;
float thetar = pi;


//Prototype
interrupt void tx_isr(void);

void main()
{

	init_DSK();
	init_Mcbsp1();
	init_Ints();

	IRQ_reset(IRQ_EVT_XINT1);
	IRQ_map(IRQ_EVT_XINT1,11);
	IRQ_nmiEnable();
	IRQ_globalEnable();
	IRQ_enable(IRQ_EVT_XINT1);
	IRQ_hook(11,tx_isr);


	MCBSP_write(DSK6713_AIC23_DATAHANDLE, scale);

	while(1)
	{

	}

} // end main



interrupt void tx_isr(void){


	AIC_data.lr[0] = scale*sin(thetal);
	AIC_data.lr[1] = scale*sin(thetar);

	thetal += delta_left;
	thetar += delta_right;


	if(thetal > 2.0*pi){
		thetal -=  2*pi;

	}

	if(thetar > 2.0*pi){

		thetar -=  2*pi;
	}


	MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.two);
}

