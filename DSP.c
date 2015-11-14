/*
 * Example program to illustrate the use of the ECE 486 interface.
 * 
 * An input waveform is squared, and streamed to the output DAC.  The 
 * waveform is also copied (unchanged) to the other DAC channel.  Each
 * USER button press inverts the signal on the second DAC.
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

int main(void)
{
  int nsamp, i;
  float *input, *output1, *output2;
  static float sign=1.0;
  static int button_count = 0;
  char outstr[100];

  initialize(FS_50K, MONO_IN, STEREO_OUT); 	// Set up the DAC/ADC interface
  
  /*
   * Allocate Required Memory
   */
  nsamp = getblocksize();
	
  input = (float *)malloc(sizeof(float)*nsamp);
  output1 = (float *)malloc(sizeof(float)*nsamp);
  output2 = (float *)malloc(sizeof(float)*nsamp);
  
  if (input==NULL || output1==NULL || output2==NULL) {
    flagerror(MEMORY_ALLOCATION_ERROR);
    while(1);
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
      output1[i] = input[i];
      output2[i] = 2 * input[i];
    }  
    DIGITAL_IO_RESET();	// (falling edge....  done processing data )
    
    /*
     * pass the processed working buffer back for DAC output
     */
    putblockstereo(output1, output2);
    
//    if(UserButtonPressed==Button_Pressed) {		// Button Press?
//      
//      sign *= -1.0;					// Invert output2
//      
//      UserButtonPressed = Button_Ready;
//
//      sprintf(outstr,"Button Pressed!  count = %x\n",button_count++); // %d, %f seem to be buggy
//      UART_putstr(outstr);
//      
//    }
  }
}
