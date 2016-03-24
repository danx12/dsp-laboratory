/*
 * non-rt-fft.c
 *
 *  Created on: Mar 24, 2016
 *      Author: villarrealh
 */

#include "complex.h"
#include <math.h>

extern void fft();

#define sampling_rate 8000.0
#define fo 1000.0
#define pi 3.141592653589
#define N 512

/* phase increment for sine wave */
float delta1= 2.0*pi*fo/sampling_rate;
float delta2 = 2.0*pi*((1000 + 0.5*(sampling_rate/N)))/N;

float theta = 0;

complex xn[N];
float mag[N];

int M = 9;


void main()
{

	init_DSK();


	while(1){
		generate_sine(xn,N);
		fft(xn,M);
		int i = 0;
		for(int i = 0; i < N; i++){
			mag[i] = pow((double) (sineArray[i]).real),2) + pow((double) (sineArray[i]).imag,2);
		}
		for(i = 0; i < 5; i++){
			// dn
		}

	}

} 



void generate_sine(complex* sineArray, int size){

	int i = 0;
	for(int i =0; i < size; i++){
		(sineArray[i]).real = sin(theta);
    	(sineArray[i]).imag = 0.0;


		theta += delta1;

		if(theta > 2.0*pi){
			theta -=  2*pi;
		}
	}
}

