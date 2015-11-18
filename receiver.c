

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

//  Need to check what I can remove for comments and still satasfy the 
//  license. 


// TODO
// Format to make it look pretty




//  Jacob Turcotte and Samuel Wallace
//  Receiver program

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>       //These two libraries are needed to make display work
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define CLOSED LOW
#define OPEN HIGH
Adafruit_SSD1306 display(OLED_RESET);




// NEED TO SET PINS
int a = 2;          	// Pin for tremolo
int b = 3;          	// Pin for Flange
int c = 4;          	// Pin for both effects
int d = 5;          	// Relay D
int e = 6;          	// Relay E
int f = 7;				// Relay F
int fl = 0;         	// Flag variable
char character = '\0';	// Used when reading in data
String effect = "";		// Stores state of device

void setup()   {  

    //Begin Serial comms at default of BT device
    Serial.begin(9600);

    // Screen init
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  
    //The following adafruit display is to comply with licensing.
    display.display();
    delay(1000);
    
  
    // Set all relay pins to outputs
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);
    pinMode(e, OUTPUT);
    pinMode(f, OUTPUT);

    // Set all relays to open when starting out
    // The relays are active low. Set pins high to turn them off.
    digitalWrite(a, OPEN);
    digitalWrite(b, OPEN);
    digitalWrite(c, OPEN);
    digitalWrite(d, OPEN);
    digitalWrite(e, OPEN);
    digitalWrite(f, OPEN);

    // Clear the display
    display.clearDisplay();

    // Display 
    display.setTextSize(2);         	//  Sets larger font
    display.setTextColor(WHITE);    	//  Doesn't matter too much. Color decided by location
    display.setCursor(0,16);        	//  Sets cursor below yellow part of display
    display.println(" Flange ");    	//  Display Flange
    display.setTextSize(2);         	//  Same size as before
    display.setTextColor(WHITE);    	//  Doesn't matter too much. Color decided by location
    display.println(" Tremolo");    	//  Tremolo
    display.display();
    delay(500);

}
 
void loop() {
  
	//Reset variables needed
	effect = "";						// Sets effect to empty for new data
	character = '\0';					// Sets character to NULL to make sure data will not be corrupt

  // Read in data
	while(Serial.available()) {
		character = Serial.read();
		effect.concat(character);		// Chars are read in by read() and then added to effect string
		delay(75);      				// Added delay or the string would not concat. 
	}
	fl = 0;								// Sets flag so it doesn't send information multiple times

 // if (effect != "") {
 //   Serial.println(effect);
 // }
  

  // Input of 0 will turn off all effects
	if (effect == "*12|99|99|0#" || effect == "0"){


  		//Screen
		display.clearDisplay();
		display.setTextSize(2);         
		display.setTextColor(WHITE);    	// Normal because effect off
		display.setCursor(0,16);        
		display.println(" Flange ");    
		display.setTextSize(2);         
		display.setTextColor(WHITE);    	// Normal because effect off
		display.println(" Tremolo");   
		display.display();
    
		// Relays
		digitalWrite(a, OPEN);
		digitalWrite(b, OPEN);
		digitalWrite(c, OPEN);
		digitalWrite(d, OPEN);
		digitalWrite(e, CLOSED);
		digitalWrite(f, OPEN);
    
		// Prints back effect
		if(fl == 0){    
			Serial.println("Clean\n");
			fl = 1;      
		}
	}
  

  // Input of 1 will turn on Flange effect
  if (effect == "*12|99|99|1#" || effect == "1"){

  	// Display
    display.clearDisplay();
    display.setTextSize(2);         
    display.setTextColor(BLACK, WHITE);	// Inverted Flange to show effect is on
    display.setCursor(0,16);        
    display.println(" Flange ");      
    display.setTextSize(2);
    display.setTextColor(WHITE);    	// Normal because effect is off
    display.println(" Tremolo");
    display.display();
    
    // Relays
    digitalWrite(a, OPEN);
    digitalWrite(b, CLOSED);
    digitalWrite(c, OPEN);
    digitalWrite(d, OPEN);
    digitalWrite(e, OPEN);
    digitalWrite(f, CLOSED);
    
    // Prints back active effect
    if(fl == 0){    
      Serial.println("Flange\n");
      fl = 1;      
    }
  }


  // Input of 2 will turn on both effects
  if (effect == "*12|99|99|2#" || effect == "2"){

  	// Display
    display.clearDisplay();
    display.setTextSize(2);         
    display.setTextColor(BLACK, WHITE);	// Inverted to show effect is active
    display.setCursor(0,16);        
    display.println(" Flange ");      
    display.setTextSize(2);         
    display.setTextColor(BLACK, WHITE);	// Inverted to show effect is active
    display.println(" Tremolo");     
    display.display();
    
    // Relays
    digitalWrite(a, CLOSED);
    digitalWrite(b, OPEN);
    digitalWrite(c, CLOSED);
    digitalWrite(d, OPEN);
    digitalWrite(e, OPEN);
    digitalWrite(f, CLOSED);
    
    // Prints back active effects
    if(fl == 0){    
      Serial.println("Both Effects\n");
      fl = 1;      
    }
  }
  

  // Input of 3 will turn on Tremolo effect
  if (effect == "*12|99|99|3#" || effect == "3"){
  	
  	// Display
    display.clearDisplay();
    display.setTextSize(2);            //  Sets larger font
    display.setTextColor(WHITE);  
    display.setCursor(0,16);            //  Sets cursor below yellow part of display
    display.println(" Flange ");          //  Display Flange
    display.setTextSize(2);             //  Same size as before
    display.setTextColor(BLACK, WHITE);        //  Doesn't matter too much. Color decided by location
    display.println(" Tremolo");         //  Tremolo
    display.display();
    
    // Relays
    digitalWrite(a, CLOSED);
    digitalWrite(b, OPEN);
    digitalWrite(c, OPEN);
    digitalWrite(d, CLOSED);
    digitalWrite(e, OPEN);
    digitalWrite(f, OPEN);
    
    // Print back active effects
    if(fl == 0){    
      Serial.println("Tremolo\n");
      fl = 1;      
    }
  }

  

}

            


        