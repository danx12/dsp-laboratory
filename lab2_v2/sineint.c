/*
 * sineint.c
 *
 *  Created on: Feb 22, 2016
 *      Author: villarrealh
 */


#include <stdio.h>
#include <stdlib.h>
#include <dsk6713.h>
#include <dsk6713_aic23.h>
#include <intr.h>
Uint32 samprate = DSK6713_AIC23_FREQ_16KHZ;

extern far void vectors();  //  this is needed to find the vecs.asm file for int processing
void init_DSK(void);
void init_Mcbsp1(void);
void init_Ints(void);


#define sampling_rate 16000.
#define freq_left 1000.
#define freq_right 2000.
#define scale 10000.0
#define PI 3.141592653589

float twopi = 2.*PI;

/* phase increment left for sine wave */
float delta_left = 2.0*PI*freq_left/sampling_rate;
/* phase increment for right sine wave */
float delta_right = 2.0*PI*freq_right/sampling_rate;
interrupt void tx_isr(void); /* prototype the ISR */


union {
	Uint32 two;
	short lr[2];
}  AIC_data;


void main(void){

	/**********************************************************************/
	/* Initialize interrupt system with intr_reset() */
	/* */
	/* The default interrupt service routines are set up by calling the */
	/* function intr_reset() in the UMD added file intr.c. This clears */
	/* GIE and PGIE, disables all interrupts except RESET in IER, clears */
	/* the flags in the IFR for the the maskable interrupts INT4 - INT15, */
	/* resets the interrupt multiplexers, initializes the interrupt */
	/* service table pointer (ISTP), and causes the Interrupt Service */
	/* Table and Interrupt Service Routine Jump Table to be loaded. */
	/**********************************************************************/
	intr_reset();
	/* dsk6713_init() must be called before other BSL functions */
	Uint32 samprate = DSK6713_AIC23_FREQ_16KHZ;

	extern far void vectors();  //  this is needed to find the vecs.asm file for int processing
	void init_DSK(void);
	void init_Mcbsp1(void);

	/* Select McBSP1 transmit int for INT15 */
	INTR_map(CPU_INT15, ISN_XINT1);
	/* Hook our ISR to INT15 */
	INTR_hook(tx_isr, CPU_INT15);
	/* Clear old interrupts */
	INTR_CLR_FLAG(CPU_INT15);
	/* Enable interrupts */
	/* NMI must be enabled for other ints to occur */
	INTR_ENABLE(CPU_INT_NMI);
	/* Set INT15 bit in IER */
	INTR_ENABLE(CPU_INT15);
	/* Turn on enabled ints */
	INTR_GLOBAL_ENABLE();
	/* Write a word to start transmission using CSL function */
	MCBSP_write(DSK6713_AIC23_DATAHANDLE, 0);
	while(1){

	}
}


interrupt void tx_isr(void){

	/**********************************************************************/
	/* Note: angle_left and angle_right must retain their values between */
	/* ISR calls. Do this by making them static as below or global. */
	/**********************************************************************/
	static float angle_left=0.;
	static float angle_right=0.;



	AIC_data.lr[0] = scale*sin(angle_left);
	AIC_data.lr[1] = scale*sin(angle_right);

	angle_left += delta_left;
	angle_right += delta_right;

	if(angle_left > 2*PI){
		angle_left -=  2*PI;
	}

	if(angle_right > 2*PI){
		angle_right -=  2*PI;
	}

	/* Put your code here to do the following:
1. Generate scaled left and right channel sine samples. Convert them
to integers and combine them into a 32-bit output word.
2. Increment phase angles of sines modulo 2*pi.
3. There is no need to poll XRDY1 since its transition from false to
true causes a jump to this ISR. DSK6713_AIC23_DATAHANDLE is
declared as a global variable in DSK6713_aic23_opencodec.c. Just
write the output sample to McBSP1 by the CSL library function
MCBSP_write() as shown below. */
	MCBSP_write(DSK6713_AIC23_DATAHANDLE, AIC_data.two);
}
