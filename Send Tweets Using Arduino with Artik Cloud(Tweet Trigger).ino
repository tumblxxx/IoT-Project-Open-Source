//variable that will not change
const int  buttonPin = 4;    // the pin that the button is attached to
const int  ldrPin = 3;        // the pin that the Light Dependent Resistor (LDR) is attached to
const int ledPinRed = 13;       // the pin that the LED (Red) is attached to
const int ledPinGreen =9;       // the pin that the LED (Green) is attached to

// Variables that will change
int buttonState = 0;         // current state of the button
int ldrState = 0;         // current state of the LDR
int lastButtonState = 0;     // previous state of the button
int lastLdrState = 0;     // previous state of the LDR

//assign pinModes in your arduino for INPUTS AND OUTPUTS
void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  Serial.begin(9600); // for sending data to python, '9600' is default
}


void loop() {
  buttonState = digitalRead(buttonPin); // get value of the button
  ldrState = digitalRead(ldrPin);   // get value of the LDR
  
  digitalWrite(ledPinRed,HIGH); //always set RED LED voltage to HIGH / on

// Condition for Button
 if (buttonState != lastButtonState) {
    if (buttonState == HIGH) { // check if button is on
      digitalWrite(ledPinGreen,HIGH); // set Green LED on
      digitalWrite(ledPinRed,LOW); // set Red LED off
      Serial.write("1"); // Send '1' to python to send 'Im home... tweet'
      delay(3000); // make green LED on & red LED off for 3 seconds
    }else {
      digitalWrite(ledPinRed,HIGH); // set Red LED on
      digitalWrite(ledPinGreen,LOW); // set Green LED off
    }
    digitalWrite(ledPinGreen,LOW); // set Green LED off
    delay(50); // delay to avoid bouncing / sending redundant data
  }
  lastButtonState = buttonState; // copy current button state for checking
  
// Condition for LDR
  if (ldrState != lastLdrState) {
    if (ldrState == HIGH) { // check if LDR is on
      digitalWrite(ledPinGreen,HIGH); // set Green LED on
      digitalWrite(ledPinRed,LOW); // set Red LED off
      Serial.write("2"); // Send '2' to python to send 'Im busy... tweet'
      delay(3000);  // make green LED on & red LED off for 3 seconds
    }else {
      digitalWrite(ledPinRed,HIGH); // set Red LED on
      digitalWrite(ledPinGreen,LOW); // set Green LED off
    }
    digitalWrite(ledPinGreen,LOW); // set Green LED off
    delay(50); // delay to avoid bouncing / sending redundant data
  }
  lastLdrState = ldrState; // copy current button state for checking
}
