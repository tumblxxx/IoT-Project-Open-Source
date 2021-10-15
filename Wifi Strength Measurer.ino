#include "arduino_secrets.h"
#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>
#include "secret.h" // SSID and PASSWORD definitions

MKRIoTCarrier carrier;
bool CARRIER_CASE = false;

int rssi = 0; // Wifi signal strengh variable
int wifi_quality_limit = -60; // Value from which you define a "good" signal
int wifi_quality_extrem = -90; // Value from which you define a "very bad" signal

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial);

// Set a welcome screen while connecting
  carrier.begin();
  carrier.leds.begin();
  carrier.display.setRotation(0);
  carrier.display.fillScreen(ST77XX_WHITE);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(4);
  carrier.display.setCursor(50,100);
  carrier.display.println("HELLO");
  carrier.display.setTextSize(2);
  carrier.display.setCursor(50,150);
  carrier.display.println("Connection...");

// attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }

//When connection successful :
  Serial.println("You're connected to the network");
  Serial.println("----------------------------------------");

//Printing on display screen
  carrier.display.fillScreen(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.setCursor(30,100);
  carrier.display.print("Connected to");
  carrier.display.setCursor(30,150);
  carrier.display.print(WiFi.SSID());
  delay(1000);
}

void loop() {
  print_data();
  update_leds();
  delay(1000);
}

// Function defining the display up to the signal quality
void print_data() {
  rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
  //define quality cases
  if(rssi>=wifi_quality_limit){                                  //if signal is good : green display
    carrier.display.fillScreen(ST77XX_GREEN);
  }
  else if(rssi<wifi_quality_limit && rssi>wifi_quality_extrem){  //if signal is medium : orange display
     carrier.display.fillScreen(ST77XX_ORANGE);
  }
  else{
    carrier.display.fillScreen(ST77XX_RED);                     // if bad signal : red display + buzzer sound
    carrier.Buzzer.sound(880);
    delay(100);
    carrier.Buzzer.noSound();
  }
  //printing display
  carrier.display.setTextSize(2);
  carrier.display.setCursor(30,80);
  carrier.display.print("Signal Strengh");
  carrier.display.setTextSize(4);
  carrier.display.setCursor(50,100);
  carrier.display.print(rssi);
  carrier.display.print("dB"); 
}

void update_leds(){
  int color_scale = map(rssi,wifi_quality_extrem,wifi_quality_limit,0,255); // create a scale from red to green depending on signal quality
  if (color_scale>255){color_scale = 255;}
  if (color_scale<0){color_scale = 0;}
  Serial.print(color_scale);
  int r = color_scale;
  int g = 255 - color_scale;
  carrier.leds.fill(carrier.leds.Color(r,g,0));
  carrier.leds.setBrightness(70); //reduce brightness 
  carrier.leds.show();
}
