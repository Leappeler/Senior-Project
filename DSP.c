/*
 * Jacob Turcotte and Samuel Wallace
 * 
 * This program will be on the board that handles the FLANGE effect.
 * The two ADC's will handle reading in a pot and the input signal.
 * The two DAC's will output the flanged input signal and just what is added to the input signal.
 * The second part of that will be done much for curiosity. 
 */

#include "stm32f4xx_hal.h"

#ifdef STM32F407xx
#include "stm32f4_discovery.h"
#endif /* STM32F407xx */

#ifdef STM32F429xx
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#endif /* STM32F429xx */

#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * Prototypes
 */

// Personal note: See Arraypractice.c for help if you get confused.
int timekeeper(float* a, float val, int *hindex, int delay, float* k);


int main(void)
{

  /*
   * Declares here
   * Add input1 and input2 when doing final. For now just make the delay work.
   */

  int nsamp, i, index = 0, delay, increment;
  float *input1, *output1, *output2, *history, dval, *input2;
  static int button_count = 0;
  char outstr[100];
 

  /*
   * Initializes the board to use one input in, two out, and use a 
   * sampling frequency of 50ksps
   */
  initialize(FS_50K, STEREO_IN, STEREO_OUT);
  
  /*
   * Allocate Required Memory
   */
  nsamp = getblocksize();
	
  input1 = (float *)malloc(sizeof(float)*nsamp);
  input2 = (float *)malloc(sizeof(float)*nsamp);
  output1 = (float *)malloc(sizeof(float)*nsamp);
  output2 = (float *)malloc(sizeof(float)*nsamp);
  history = (float *)malloc(sizeof(float)*12000);

  if (input1==NULL || output1==NULL || output2==NULL || history == NULL || input2 == NULL) {
    flagerror(MEMORY_ALLOCATION_ERROR);
    while(1);
  }
  
  for (i = 0; i < 12000; i++){
    history[i] = 0;
  }

  // .5 ms delay to start roughly
  delay = 5;
  increment = 1;
  /*
   * Infinite Loop to process the data stream, "nsamp" samples at a time
   */
  while(1){
    /*
     * Ask for a block of ADC samples to be put into the working buffer
     *   getblock() will wait until the input buffer is filled...  On return
     *   we work on the new data buffer.
     */
    getblockstereo(input1, input2);	// Wait here until the input buffer is filled... Then process	

    /*
     * signal processing code to calculate the required output buffers
     */
    
    for (i=0; i<nsamp; i++) {

      
      timekeeper(history, input1[i], &index, delay, &dval);
      // Added half each signal because adc reads -1 - 1. Adding both signals
      //Throws out of bounds and fun things happen. This makes dac output 0 - 3 volts.
      //output1[i] = (input1[i]/2) + (dval/2);
      //output2[i] = input2[0];
      output1[i] = input1[i];
      output2[i] = dval;
    } 
    
    /*
     * pass the processed working buffer back for DAC output
     */
    putblockstereo(output1, output2);

    // changing the delay by the increment
    delay += increment;

    //Setting upper limit
    if (((float) delay) > ((input1[0] + 1) * 50) + 50){
      increment = -increment;
    }

    //Setting upper limit
    if (delay < 5){
      increment = -increment;
    }



    for (i = 0; i  < nsamp; i++){
      output1[i] = 0;
      output2[i] = 0;
      input1[i] = 0;
      input2[i] = 0; 
    
    }


  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * User defined functions
 */



int timekeeper(float *a, float val, int *hindex, int delay, float* k)
{

  // Delayed index is equal to the current history index minus time delay (5 * 500 = 2500)
  int dindex = *hindex - (delay * 5);

  //turns negative index to one on end of array
  if (dindex < 0) {
    dindex += 12000;
  }

  //Takes care of null case
  if (dindex == 12000){
    dindex = 0;
  }



  a[*hindex] = val;
  *hindex += 1;

  if (*hindex < 0) {
    *hindex += 12000;
  }

  //Takes care of null case
  if (*hindex == 12000){
    *hindex = 0;
  }

  *k = a[dindex];
  return 0;
}