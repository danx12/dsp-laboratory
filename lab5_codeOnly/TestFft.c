
/***************************************************************/ /* Program testfft.c */ /* An example of how to use function fft.c */ /* Compile by: gcc testfft.c fft.c -o fft.out -lm */ /***************************************************************/
#include "complex.h"
#include <math.h>
extern void fft();
main() {
/* Declare input array              */
/* loop index                       */
/* log2(16)                         */
complex X[16];
int i;
int M = 4;
float pi = 3.141592653589;
int N = 16;              /*  Number of FFT points            */
/*-------------------------------------------------------------*/ /* Initialize input array */ /* Generate spectral lines at k = 5 and 11 of height 8. */
  for(i=0; i<N; i++)
    {
    (X[i]).real = cos(i*5*2.0*pi/N);
    (X[i]).imag = 0.0;
    }
 /*--------------------------------------------------------------*/ /* Perform FFT */
fft(X,M);
/* Display results on screen */
  for(i=0; i<N; i++)
 printf("%4d%15.5f\t%15.5f\n",i,(X[i]).real, (X[i]).imag);
}