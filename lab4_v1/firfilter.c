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
#define scale 10000.0
#define N 25


float h[] = {
		   0.003706141310013,
		   0.012980544885507,
		   0.003334817395545,
		   0.018295048435348,
		   0.020085182312932,
		  -0.017853205317284,
		   0.016481878399746,
		  -0.031409926462122,
		  -0.096666554718863,
		  -0.002591559392632,
		  -0.175830500296683,
		  -0.184741536921730,
		   0.436152434411172,
		   0.436152434411172,
		  -0.184741536921730,
		  -0.175830500296683,
		  -0.002591559392632,
		  -0.096666554718863,
		  -0.031409926462122,
		   0.016481878399746,
		  -0.017853205317284,
		   0.020085182312932,
		   0.018295048435348,
		   0.003334817395545,
		   0.012980544885507,
		   0.003706141310013
};

float xLeft[25];
float xRight[25];

int newest = N;



//Prototype
interrupt void tx_isr(void);

void main()
  {

	init_DSK();
	init_Mcbsp1();
	init_Ints();

	MCBSP_write(DSK6713_AIC23_DATAHANDLE, scale);

	while(1)
	{

	}

} // end main



interrupt void tx_isr(void){


	//Read data from McBSP1
	AIC_data.two = MCBSP_read(DSK6713_AIC23_DATAHANDLE);
	//AIC_data.lr[1] = AIC_data.lr[0];

	float leftChannel = (float) AIC_data.lr[0];
	float rightChannel = (float) AIC_data.lr[1];

	newest--;
	if(newest == -1){
		newest = N-1;
	}
	//add new sample
	xLeft[newest] = leftChannel;
	xRight[newest] = rightChannel;

	//do convolution
	float yLeft;
	float yRight;
	int xInd = newest;
	int i;

	for(i = 0; i < N; i++){
		yLeft += h[i] * xLeft[xInd];
		yRight += h[i] * xRight[xInd];
		xInd++;
		if(xInd == N){
			xInd = 0;
		}
	}

	//AIC_data.two = 0;
	AIC_data.lr[0] = AIC_data.lr[1]; //red
	AIC_data.lr[1] = (int) (yRight); //white

	MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.two);
}

