/*

This code uses 1Sheeld and an arduino Uno board to control a 4x4x4 LED matrix using 
  the microphone of your smart phone.
 
 The circuit:
 * 64 LEDs form a 4x4x4 cube connected with the 1Sheeld and arduino board  
 * The coloumns control pins are connected to digital pins  0 to 13 and analogue pins 
     A4 and A5
 * The layers control pins are connected to analouge pins  A0 to A3.   
 
 
 by Hassan Ibrahim

*/
  
   
define CUSTOM_SETTINGS
define INCLUDE_PUSH_BUTTON_SHIELD
define INCLUDE_MIC_SHIELD

/* Include 1Sheeld library. */
#include <OneSheeld.h>
 

// Define the pin connections of the LED matrix (columns and layer).
// Refer to the tutorial for more illustration.
int coloumn13 = 13;     // coloumn13 means the coloumn connected to pin 13 on the arduino board 
int coloumn12 = 12;
int coloumn11 = 11;
int coloumn10 = 10;
int coloumn9 = 9;
int coloumn8 = 8;
int coloumn7 = 7;
int coloumn6 = 6;
int coloumn5 = 5;
int coloumn4 = 4;
int coloumn3 = 3;
int coloumn2 = 2;
int coloumn1 = 1;
int coloumn0 = 0;
int coloumnA4 = A4;
int coloumnA5 = A5;

int layerA0 = A0;      // layerA0 means the layer connected to analogue pin A0 on the arduino (The lowest layer in this example)
int layerA1 = A1;      // * The names are chosen this way in order to ease modification
int layerA2 = A2;
int layerA3 = A3;

int counter;   // used to make variations in the coloumns

// the setup routine runs once when you press reset:
void setup() {                
  // Start communication. 
    OneSheeld.begin();
    Serial.begin(115200);
 // initialize each pin as an output. 
    pinMode(coloumn0, OUTPUT);
    pinMode(coloumn1, OUTPUT);
    pinMode(coloumn2, OUTPUT);
    pinMode(coloumn3, OUTPUT);
    pinMode(coloumn4, OUTPUT);
    pinMode(coloumn5, OUTPUT);
    pinMode(coloumn6, OUTPUT);
    pinMode(coloumn7, OUTPUT);
    pinMode(coloumn8, OUTPUT);
    pinMode(coloumn9, OUTPUT);
    pinMode(coloumn10, OUTPUT);
    pinMode(coloumn11, OUTPUT);
    pinMode(coloumn12, OUTPUT);
    pinMode(coloumn13, OUTPUT);
    pinMode(coloumnA4, OUTPUT);
    pinMode(coloumnA5, OUTPUT);
    pinMode(layerA0, OUTPUT);
    pinMode(layerA1, OUTPUT);
    pinMode(layerA2, OUTPUT);
    pinMode(layerA3, OUTPUT);
}   // end of setup



// the loop routine runs over and over again forever:
void loop() {
  
  // all coloumns are connected to 5v (high).
    digitalWrite(coloumn0,HIGH);
    digitalWrite(coloumn1,HIGH);
    digitalWrite(coloumn2,HIGH);
    digitalWrite(coloumn3,HIGH);
    digitalWrite(coloumn4,HIGH);
    digitalWrite(coloumn5,HIGH);
    digitalWrite(coloumn6,HIGH);
    digitalWrite(coloumn7,HIGH);
    digitalWrite(coloumn8,HIGH);
    digitalWrite(coloumn9,HIGH);
    digitalWrite(coloumn10,HIGH);
    digitalWrite(coloumn11,HIGH);
    digitalWrite(coloumn12,HIGH);
    digitalWrite(coloumn13,HIGH);
    digitalWrite(coloumnA4,HIGH);
    digitalWrite(coloumnA5,HIGH);
    delay(30);

    
    // Get the noise level from the mic and control which layers to be connected to 0v (low).
    // Each if-else block controls one layer.
    // The values should be calibrated according to your mobile phone.
    /* 
         To calibrate, play any sort of music near your smart phone and watch the            readings of the Mic shield and choose appropriate values.
    */
    // In this example, steps of 6 give good results
     
     if( Mic.getValue() > 70 )  // first layer control
      { 
          digitalWrite(layerA0, LOW);
            delay(50);                 // To make the display clearer. It could be modified to make if faster or slower.
      }
      else 
      {
        digitalWrite(layerA0, HIGH);
      }
       
      if( Mic.getValue() > 76 )   // second layer control
      { 
          digitalWrite(layerA1, LOW);
            delay(50);
      }
      else 
      {
        digitalWrite(layerA1, HIGH);
      }
      
       if(   Mic.getValue() > 82 )   // third layer control
      { 
          digitalWrite(layerA2, LOW);
            delay(50);
      }
      else 
      {
        digitalWrite(layerA2, HIGH);
      }
      
       if(   Mic.getValue() > 88 )   // fourth layer control
      { 
          digitalWrite(layerA3, LOW);
            delay(50);
      }
      else 
      {
        digitalWrite(layerA3, HIGH);
      }
    
  
  } // end of loop
  
 
