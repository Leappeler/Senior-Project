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
float timekeeper(float* a, float val, int *hindex, int delay);


int main(void)
{

  /*
   * Declares here
   * Add input1 and input2 when doing final. For now just make the delay work.
   */

  int nsamp, i, index = 0, delay;
  float *input, *output1, *output2, *history, dval;
  static int button_count = 0;
  char outstr[100];
 

  /*
   * Initializes the board to use one input in, two out, and use a 
   * sampling frequency of 50ksps
   */
  initialize(FS_50K, MONO_IN, STEREO_OUT);
  
  /*
   * Allocate Required Memory
   */
  nsamp = getblocksize();
	
  input = (float *)malloc(sizeof(float)*nsamp);
  output1 = (float *)malloc(sizeof(float)*nsamp);
  output2 = (float *)malloc(sizeof(float)*nsamp);
  history = (float *)malloc(sizeof(float)*12000);

  if (input==NULL || output1==NULL || output2==NULL || history == NULL) {
    flagerror(MEMORY_ALLOCATION_ERROR);
    while(1);
  }
  
  for (i = 0; i < 12000; i++){
    history[i] = 0;
  }
  /*
   * Infinite Loop to process the data stream, "nsamp" samples at a time
   */
  while(1){
    /*
     * Ask for a block of ADC samples to be put into the working buffer
     *   getblock() will wait until the input buffer is filled...  On return
     *   we work on the new data buffer.
     */
    getblock(input);	// Wait here until the input buffer is filled... Then process	

    /*
     * signal processing code to calculate the required output buffers
     */
    
    DIGITAL_IO_SET(); 	// Use a scope on PC4 to measure execution time
    for (i=0; i<nsamp; i++) {

      delay = 5;
      dval = timekeeper(history, input[i], &index, delay);
      output1[i] = input[i];
      output2[i] = dval;
      //output2[i] = input[i];
    }  
    DIGITAL_IO_RESET();	// (falling edge....  done processing data )
    
    /*
     * pass the processed working buffer back for DAC output
     */
    putblockstereo(output1, output2);
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * User defined functions
 */



float timekeeper(float *a, float val, int *hindex, int delay)
{

  // Delayed index is equal to the current history index minus time delay (5 * 500 = 2500)
  int dindex = *hindex - (delay * 500);

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

  return a[dindex];
}