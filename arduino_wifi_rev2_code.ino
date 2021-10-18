//#define BLYNK_PRINT Serial // debug output for Blynk

#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

#define GREEN_LED_pin   26 // pins for the RGB LED (built in to the Arduno)
#define BLUE_LED_pin    27
#define RED_LED_pin     25

#include <Blynk.h>                  // necessary libraries
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
WidgetBridge bridge1(V0);           // bridge widge for receiving data

char auth[] = "Auth token"; // auth, ssid, and pass
char ssid[] = "SSID";
char pass[] = "Password";

int receivedVal;              // received data
bool relayOn = false;         // is the relay on?
bool lastRelayState = false;  // last relay state
const int relayPin = 2;       // relay pin
const int buttonPin = 3;      // button pin for manual control
const int reconnectPin = 4;   // button to reconnect
bool canReconnect = true;    // default to false, changeable by button press
/*   CONNECTIONS
  RELAY ----- Arduino
  +     ----- 5V
  -     ----- GND
  S     ----- D2
  
  RELAY                 -----    Light (can be LED, etc)
  
  NC (normally closed)  -----    Nothing
  Middle pin            -----    One pin of light
  NO (normally open)    -----    Power (wall, +5V, etc)
  Normally open means that when the relay does not have power, the circuit is open, meaning the light is off. I decided to use this side because I wanted a saftey, like if my Arduino loses power, short circuits, or anything else bad, it would stop powering the relay, meaning the lights would go off.
  
  LED connections (using LED as an example)
  shorter end (GND)  ----- 220 ohm resistor ----- GND
  longer end (power) ----- +5V
  Use 10 ohm resistor for 3.3V power
*/

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_7x14B_tf);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  // set up RGB LED (buitlin LED)
  WiFiDrv::pinMode(RED_LED_pin, OUTPUT);   //RED
  WiFiDrv::pinMode(GREEN_LED_pin, OUTPUT); //GREEN
  WiFiDrv::pinMode(BLUE_LED_pin, OUTPUT);  //BLUE

  //turn off RGB led
  WiFiDrv::digitalWrite(RED_LED_pin, LOW);
  WiFiDrv::digitalWrite(GREEN_LED_pin, LOW);
  WiFiDrv::digitalWrite(BLUE_LED_pin, LOW);
  
  pinMode(relayPin, OUTPUT);      // set up relay
  pinMode(buttonPin, INPUT);      // set up both buttons
  pinMode(reconnectPin, INPUT);
  Serial.begin(9600);             // begin Serial
  setLEDColor(0, 255, 255);         // set LED color to red
  clear();
  textAt(25, 25, "Connecting");
  send();
  Blynk.begin(auth, ssid, pass);  // begin Blynk
  while (!Blynk.connected());     // wait for Blynk connections
  setLEDColor(0, 255, 0);         // set the LEDcolor to green (connected now!)
  clear();
  textAt(25, 25, "Connected");
  send();
  delay(5000);
  clear();
  send();
}
bool lastState, currentState;
void loop() {
  Blynk.run(); // handle Blynk
  currentState = Blynk.connected();
  if (currentState){ // set LED color based on WiFi connections
    setLEDColor(0, 255, 0);  // green (connected)
    if (digitalRead(buttonPin) == HIGH){
      bridge1.virtualWrite(V6, 1);
      delay(1500);
    }
  }
//  Serial.println(
  if (digitalRead(reconnectPin) == HIGH){
    canReconnect = !canReconnect;
    clear();
    textAt(5, 30, "Auto reconnect:");
    if (canReconnect){
      textAt(5, 60, "Yes");
    }
    else{
      textAt(5, 60, "No");
    }
    send();
    delay(2000);
    clear();
    send();
  }
  if (!currentState){
    setLEDColor(255, 0, 0);
    if (canReconnect){
      clear();
      textAt(5, 30, "Reconnecting...");
      send();
      while (!Blynk.connected()){
        Blynk.begin(auth, ssid, pass);
        manualControl();
      }
      clear();
      textAt(5, 30, "Reconnected!");
      send();
      setLEDColor(255, 0, 0);
      currentState = true;
      delay(5000);
      clear();
      send();
    }
    else{
      manualControl();
      delay(1000);
    }
  }
  Blynk.virtualWrite(V2, rssiPercent());
//  Serial.println(rssiStrength);
  if (relayOn != lastRelayState){ // if the last state is not the current state
    clear();
    if (relayOn){
      textAt(5, 30, "Light: On");
    }
    else{
      textAt(5, 30, "Light: Off");
    }
    send();
  }
  if ((!currentState) and (currentState != lastState)){
    setLEDColor(255, 0, 0);
    clear();
    textAt(5, 30, "Disconnected");
    send();
  }
/*  if (!currentState){   // removed because of optional auto-reconnect
    manualControl();
    delay(100);
  }*/
  lastState = currentState;
  lastRelayState = relayOn;
}

BLYNK_WRITE(V5){ // when receiving data on V5 (make sure this is the same on the sending side)
  receivedVal = param.asInt();  // get the value
  if (receivedVal == 1){        // if it is 1
    relayOn = true;    // turn it on
  }
  else{ // else
    relayOn = false;   // turn it off
  }
  if (relayOn){ // if it is true, turn the relay on
    digitalWrite(relayPin, HIGH);
  }
  else{ // if it is false, turn the relay off
    digitalWrite(relayPin, LOW);
  }
}

void setLEDColor(uint8_t R, uint8_t G, uint8_t B){
  //R, G, and B values should not exceed 255 or be lower than 0.
  
  //set ESP32 wifi module RGB led color
  WiFiDrv::analogWrite(RED_LED_pin, R); //Red
  WiFiDrv::analogWrite(GREEN_LED_pin, G); //Green
  WiFiDrv::analogWrite(BLUE_LED_pin, B);  //Blue
}
BLYNK_CONNECTED(){ // connected, now set the auth token of the other device
  bridge1.setAuthToken("Other auth token");
}
void manualControl(){
  if (digitalRead(buttonPin) == HIGH){
    relayOn = !relayOn;
    delay(100);
  }
  digitalWrite(relayPin, relayOn);
}
void clear(){
  u8g2.clearBuffer();
}
void send(){
  u8g2.sendBuffer();
}
void textAt(int x, int y, String text){
  u8g2.setCursor(x, y);
  u8g2.print(text);
}
int rssiPercent(){
  return map(WiFi.RSSI(), -100, 0, 0, 100);
}
/*
 * This does not work:
 * Modify if you want
 * put after if (Blynk.connected()){
 * }
  else{
    setLEDColor(255, 0, 0);
    clear();
    textAt(5, 30, "Disconnected...");
    textAt(5, 60, "Reconnecting");
    send();
    bool result = Blynk.connect();
    delay(10000);
    if (result){
      textAt( ... );
    }
  }
 */
  /*
  else{
    setLEDColor(255, 0, 0);  // red (disconnected)
    clear();
    textAt(5, 30, "Disconnected");
    send();
    manualControl();
    delay(100);
  }*/
