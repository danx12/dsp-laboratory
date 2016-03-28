/*
 * sineoutirq.c
 *
 *  Created on: Feb 22, 2016
 *      Author: villarrealh
 */

#include "dsk6713_aic23.h"
#include "math.h"
Uint32 samprate = DSK6713_AIC23_FREQ_8KHZ;

extern far void vectors();  //  this is needed to find the vecs.asm file for int processing
void init_DSK(void);
void init_Mcbsp1(void);
void init_Ints(void);

union {
	Uint32 two;
	short lr[2];
}  AIC_data;


#define sampling_rate 8000.

//echo controls
#define scale 10000.0
#define N 5760

short leftBuffer[N];
short rightBuffer[N];
int index = 0;
int amplitude = 5;



//Prototype
interrupt void tx_isr(void);

void main()
  {

	init_DSK();
	init_Mcbsp1();
	init_Ints();

	

	int i =0;
	for(int i = 0; i < N; i++){
		leftBuffer[i] = 0;
		rightBuffer[i] = 0;
	}

	MCBSP_write(DSK6713_AIC23_DATAHANDLE, scale);

	while(1)
	{

	}

} // end main



interrupt void tx_isr(void){

	short leftOutput;
	short rightOutput;

	//Read data from McBSP1
	AIC_data.two = MCBSP_read(DSK6713_AIC23_DATAHANDLE);

	leftOutput = AIC_data.lr[0] + 0.1*amplitude*leftBuffer[i];
	rightOutput = AIC_data.lr[1] + 0.1*amplitude*rightBuffer[i];

	leftBuffer[i] = AIC_data.lr[0];
	rightBuffer[i] = AIC_data.lr[1];
	i++;
	if(i >= N){
		i = 0;
	}

	AIC_data.two = 0;

	AIC_data.lr[0] = leftOutput; //red
	AIC_data.lr[1] = rightOutput //white

	MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.two);
}

