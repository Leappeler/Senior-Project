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


///////////////////////////////////////////////////////////////////////////////////////////////
Code that needs to be combined for the Receiver

SET PINS TO WHAT THEY NEED TO BE

int a = 13; 		// Pin for tremolo
int b =  12;		// Pin for Flange
int c = 11;			// Pin for both effects
int d = 0; 			// Relay D
int e = 0; 			// Relay E
int effect = 0;		// Effect variable
int fl = 0; 		// Flag variable
 
void setup() {
 pinMode(a, OUTPUT);
 pinMode(b, OUTPUT);
 pinMode(c, OUTPUT);
 pinMode(d, OUTPUT);
 pinMode(e, OUTPUT);


// Set all relays to open when starting out
 digitalWrite(a, LOW);
 digitalWrite(b, LOW);
 digitalWrite(c, LOW);
 digitalWrite(d, LOW);
 digitalWrite(e, LOW);

 
 Serial.begin(9600); // Default connection rate for my BT module
}
 
void loop() {

 if(Serial.available() > 0){
 	effect = Serial.read();
 	fl = 0;			
 }

 switch (effect){

 	// Input 0 clears effects
 	case '0': {
 		digitalWrite(tremolo, LOW);

 		if(fl == 0){	
 			Serial.println("Effects cleared");
 			fl = 1;		
 		}
 	}

 }







 if (effect == '0') {
 	digitalWrite(tremolo, LOW);
 	if(fl == 0){	
 		Serial.println("LED: off");
 		fl = 1;		
 	}
 }

 else if (effect == '1') {
 	digitalWrite(tremolo, HIGH);
 	if(fl == 0){
 		Serial.println("LED: on");
 		fl = 1;		//Added to debug code
 	}
 }

 //Let's make a case statement


}