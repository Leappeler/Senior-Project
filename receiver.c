/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

//	Need to check what I can remove for comments and still satasfy the 
//	license. 




//	Jacob Turcotte and Samuel Wallace
//	Receiver program
//	At this moment in time, the display works. Figuring out how to
//	make each word (Flange), (Tremolo), (Flange + Tremolo) work. 
//	Possibly display both and just highlight the ones that are present?

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>		//These two libraries are needed to make display work
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//The following adafruit display is to comply with licensing.
display.display();
delay(1000);


void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.


  // Clear the buffer.
  display.clearDisplay();

  
  // text display tests
  display.setTextSize(2);		//	Sets larger font
  display.setTextColor(WHITE);	//	Doesn't matter too much. Color decided by location
  display.setCursor(0,16);		//	Sets cursor below yellow part of display
  display.println("Flange");	//	Display Flange
  display.setTextSize(2);		//	Same size as before
  display.setTextColor(WHITE);	//	Doesn't matter too much. Color decided by location
  display.println("Tremolo");	//	Tremolo
  display.display();
  delay(2000);



}

void loop() {
  
  
}


