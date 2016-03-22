/* The C routines to initialize the C6713 DSK */
#include "evmstart.h"

extern Uint32 samprate;

void init_DSK(void)
{
	// set up the C6713, the AIC and initialize the CSL libraries
	DSK6713_init();        // TI routine to initialize the 6713 DSK board
	//CSL_init();            // TI routine to initialize CSL libraries when not using BIOS

	// Set up AIC through control port connected to MCBSP0
	aic_control = DSK6713_AIC23_openCodec(0, &aicsettings); // pointer to aic control serial port (0)
	DSK6713_AIC23_setFreq(aic_control, samprate);            // set the aic sample rate

}

void init_Mcbsp1(void)
{
	// The data channel of the aic is hooked up to MCBSP1, renamed the datahandle by DSK makers
	MCBSP_config(DSK6713_AIC23_DATAHANDLE,&mcbspCfg1);  // Set up data mcbsp parameters

	MCBSP_start(DSK6713_AIC23_DATAHANDLE, MCBSP_XMIT_START | MCBSP_RCV_START |
			MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 220);   // start the data channel
}


void init_Ints(void)  // used to generate sinusoid with interrupts
{
	IRQ_setVecs(vectors);
	IRQ_reset(IRQ_EVT_XINT1);
	IRQ_map(IRQ_EVT_XINT1,11);
	IRQ_nmiEnable();
	IRQ_globalEnable();
	IRQ_enable(IRQ_EVT_XINT1);



	/* add your mappings to set up your interrupt (mcbsp, edma etc) here */




}

