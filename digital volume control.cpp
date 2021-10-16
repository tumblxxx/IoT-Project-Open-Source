#include <PT2258.h>
#include <ezButton.h>
#include <Wire.h>
PT2258 pt2258; // PT2258 Object
ezButton button_1(2); //Button_1 Object
ezButton button_2(4); //Button_2 Object
int volume = 40; // Default volume / Starting Volume
void setup() {
  Serial.begin(9600); //UART begin
  Wire.setClock(100000); // setting the I2C clock to 100KHz
  /* checking if the MCU can talk with the PT or not*/
  if (!pt2258.init())
    Serial.println("PT2258 Successfully Initiated");
  else
    Serial.println("Failed to Initiate PT2258");
  /* Setting up button debounce delay*/
  button_1.setDebounceTime(50);
  button_2.setDebounceTime(50);
  /* Initiating PT with default volume and Pin*/
  pt2258.setChannelVolume(volume, 4);
  pt2258.setChannelVolume(volume, 5);
}
void loop() {
  button_1.loop(); //Library norms
  button_2.loop(); //Library norms
  /* If button 1 is pressed if condition is true*/
  if (button_1.isPressed())
  {
    volume++; // Incrementing the volume counter.
    // this if statement ensures the volume does not goes above 79
    if (volume >= 79)
    {
      volume = 79;
    }
    Serial.print("volume: "); // Printing the volume level
    Serial.println(volume);
    /*Set the volume for channel 4
      Which is in PIN 9 of the PT2258 IC
    */
    pt2258.setChannelVolume(volume, 4);
    /*Set the volume for channel 5
       Which is the Pin 10 of the PT2258 IC
    */
    pt2258.setChannelVolume(volume, 5);
  }
  //The same happens for the button 2
  if (button_2.isPressed())
  {
    volume--;
    // this if statement ensures the volume level doesn't go below zero.
    if (volume <= 0)
      volume = 0;
    Serial.print("volume: ");
    Serial.println(volume);
    pt2258.setChannelVolume(volume, 4);
    pt2258.setChannelVolume(volume, 5);
  }
}
