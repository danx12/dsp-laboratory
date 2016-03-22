/* file to output a test sinusoid using evmstart.{c,h} */
/* file location is \\eceserv2\students\realtimefiles\C6713Boiler */

#include "dsk6713_aic23.h"
#include "math.h"
Uint32 samprate = DSK6713_AIC23_FREQ_8KHZ;

extern far void vectors();  //  this is needed to find the vecs.asm file for int processing
void init_DSK(void);
void init_Mcbsp1(void);
void init_Ints(void);

void main()
{

float thetal=0.0, thetar=0.0;
float pi,lstep,rstep;
float fs = 8000;
float fo1 = 1000;
float fo2 = 2000;
/*
 *  Define a union to handle the AIC stereo data words
 */
union {
  Uint32 two;
  short lr[2];
  }  AIC_data;

init_DSK();
init_Mcbsp1();
//init_IRQ()  not used here but in sineirq lab


/* initialize everything */
//pi = 3.1415926;
pi = atan(1.0)*4.0;
//
lstep = 2*pi*fo1/fs;
rstep = 2*pi*fo2/fs;

//
AIC_data.two = 0; // initialize both channels to 0, not needed since values are
                  // assigned below, before output
while(1)
  {
  /* poll the xdr register and transmit when ready */
  // get next sine values and put into output word
  // use a big gain on the sine:   10000 to 150000
	while(!MCBSP_xrdy(DSK6713_AIC23_DATAHANDLE)){

	}
	MCBSP_write(DSK6713_AIC23_DATAHANDLE,AIC_data.two);//write/output data
	thetal += lstep;
	thetar += rstep;




	if(thetal > 2.0*pi){
		thetal -=  2*pi;

	}

	if(thetar > 2.0*pi){

		thetar -=  2*pi;
	}

  // hold here until ready for next data, (use MCBSP_xrdy function as in Chassaing)
 // XXX
  // write out the data (use MCBSP_write)
 // XXX
  } // end while(1)

} // end main

