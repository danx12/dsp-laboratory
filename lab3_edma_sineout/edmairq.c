/*
 * edmairq.c
 *
 *  Created on: Mar 1, 2016
 *      Author: villarrealh
 */


#include "dsk6713_aic23.h"
#include "math.h"
#include <c6x.h>
#include <csl.h>
#include <csl_irq.h>
#include <csl_edma.h>

Uint32 samprate = DSK6713_AIC23_FREQ_16KHZ;

extern far void vectors();  //  this is needed to find the vecs.asm file for int processing
void init_DSK(void);
void init_Mcbsp1(void);
void init_Ints(void);

interrupt void tx_isr(void);
void initEdma();
void startEdma(void);
void fillBuffer(void);



#define sampling_rate 16000.
#define BUFFSIZE 1024
#define freq_left 1200.
#define freq_right 1203.
#define scale 15000.0
#define pi 3.141592653589


float delta_left = 2.0*pi*freq_left/sampling_rate;
float delta_right = 2.0*pi*freq_right/sampling_rate;

float thetal = pi;
float thetar = pi;


int gBuffer0[BUFFSIZE];


union {
	Uint32 two;
	short lr[2];
}  AIC_data;


//EDMA Variable and structures

short gXmtChan;  // holds TCC code (CIPR bit number)
EDMA_Handle hEdma;
EDMA_Handle hEdmaReload;




void main()
{
	CSL_init();
	init_DSK();
	init_Mcbsp1();
	initEdma();
	init_Ints();


	int i;
	for(i = 0; i < BUFFSIZE; i++){
		gBuffer0[i] = 0;
	}

	MCBSP_enableXmt(DSK6713_AIC23_DATAHANDLE);
	MCBSP_write(DSK6713_AIC23_DATAHANDLE, 15000);

	while(1){

	}

} // end main




void fillBuffer(void){
	int i;
	for(i = 0; i < BUFFSIZE; i++){

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

		gBuffer0[i] = 0;
		gBuffer0[i] = AIC_data.two;
	}


}





void initEdma(){


	EDMA_Config gEdmaConfig  = {
			EDMA_OPT_RMK(
					EDMA_OPT_PRI_LOW,    // Priority?
					EDMA_OPT_ESIZE_32BIT,  // Element size?
					EDMA_OPT_2DS_NO,         // 2 dimensional source?
					EDMA_OPT_SUM_INC,        // Src update mode?
					EDMA_OPT_2DD_NO,         // 2 dimensional dest?
					EDMA_OPT_DUM_NONE,        // Dest update mode?
					EDMA_OPT_TCINT_YES,   // Cause EDMA interrupt?
					EDMA_OPT_TCC_OF(0),  // Transfer complete code assined later
					EDMA_OPT_LINK_YES,   // Enable link parameters?
					EDMA_OPT_FS_NO       // Use frame sync?
			),
			EDMA_SRC_OF(gBuffer0),        // src address?
			EDMA_CNT_OF(BUFFSIZE),        // Count = buffer size
			EDMA_DST_OF(0),        // dest address?
			EDMA_IDX_OF(0),          // frame/element index value?
			EDMA_RLD_OF(0)           // reload
	};



	hEdma = EDMA_open(EDMA_CHA_XEVT1,EDMA_OPEN_RESET); // open XEVT1 channel
	gXmtChan = EDMA_intAlloc(-1);  // get an open CIPR bit (TCC code)
	gEdmaConfig.opt |= EDMA_FMK(OPT,TCC,gXmtChan); // put TCC code into struct
	gEdmaConfig.dst = MCBSP_getXmtAddr(DSK6713_AIC23_DATAHANDLE);
	EDMA_config(hEdma, &gEdmaConfig); //gEdmaConfig struct defined elsewhere

	/* now configure the reload */
	hEdmaReload = EDMA_allocTable(-1);
	EDMA_config(hEdmaReload,&gEdmaConfig);
	EDMA_link(hEdma, hEdmaReload);
	EDMA_link(hEdmaReload,hEdmaReload);

	//  This is to clear all left over EDMA ints
	EDMA_intClear(gXmtChan );
	EDMA_intEnable(gXmtChan );
	EDMA_enableChannel(hEdma);
	EDMA_setChannel(hEdma);




}




interrupt void edmaHwi(void){


	EDMA_intClear(gXmtChan);
	fillBuffer();
	//EDMA_setChannel(hEdma);


}



