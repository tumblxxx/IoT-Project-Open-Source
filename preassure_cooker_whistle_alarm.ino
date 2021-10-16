 //Developed by Desigizmo4u Pvt. Ltd.
  //product Designed by Sourav Paul 
  //Version 1.1
  //Date last updated on 05/06/18 , 07:09 AM


const int buttonPin = 13;     // the number of the pushbutton pin
const int buttonPowerPin =  2;      // the number of the LED pin
const int soundPin = 3; // the number of the sound sensor pin
int counter = 1; // set initial counter to value 1
const int buzzerPin = A1; // the number of buzzer pin
int whistle = 0; // initial whistle value to zero.
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int whistlestate = 0;
int lastwhistlestate = 0;     // previous state of the whistle
volatile int state = LOW;
boolean Counterflag = true;
unsigned long start, finished, elapsed;

//declare variables for 7 segment display
int seg_a = 11; // declare the variables
int seg_b = 10;
int seg_c = 9;
int seg_d = 8;
int seg_e = 7;
int seg_f = 6;
int seg_g = 5;
int seg_dp = 4;
int com = 12;


void blink() { 
   //ISR function
   state = !state; //toggle the state when the interrupt occurs
}

void print0()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,HIGH);
  digitalWrite(seg_f,HIGH);
  digitalWrite(seg_g,LOW);
  digitalWrite(seg_dp,LOW);
}

void print1()
{
  digitalWrite(seg_a,LOW);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,LOW);
  digitalWrite(seg_e,LOW);
  digitalWrite(seg_f,LOW);
  digitalWrite(seg_g,LOW);
  digitalWrite(seg_dp,LOW);
}
void print2()
{
   digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,LOW);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,HIGH);
  digitalWrite(seg_f,LOW);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}
void print3()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,LOW);
  digitalWrite(seg_f,LOW);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}
void print4()
{
 digitalWrite(seg_a,LOW);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,LOW);
  digitalWrite(seg_e,LOW);
  digitalWrite(seg_f,HIGH);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}
void print5()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,LOW);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,LOW);
  digitalWrite(seg_f,HIGH);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}
void print6()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,LOW);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,HIGH);
  digitalWrite(seg_f,HIGH);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}
void print7()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,LOW);
  digitalWrite(seg_e,LOW);
  digitalWrite(seg_f,LOW);
  digitalWrite(seg_g,LOW);
  digitalWrite(seg_dp,LOW);
}
void print8()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,HIGH);
  digitalWrite(seg_f,HIGH);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}

void print9()
{
  digitalWrite(seg_a,HIGH);
  digitalWrite(seg_b,HIGH);
  digitalWrite(seg_c,HIGH);
  digitalWrite(seg_d,HIGH);
  digitalWrite(seg_e,LOW);
  digitalWrite(seg_f,HIGH);
  digitalWrite(seg_g,HIGH);
  digitalWrite(seg_dp,LOW);
}



void setup() {
  
 
  digitalWrite(com,LOW);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(buttonPowerPin, OUTPUT);
  digitalWrite(buttonPowerPin, HIGH);
  Serial.begin(9600);
  pinMode(soundPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(soundPin), blink, FALLING);

  pinMode(seg_a,OUTPUT); // configure all pins used to outputs
  pinMode(seg_b,OUTPUT);
  pinMode(seg_c,OUTPUT);
  pinMode(seg_d,OUTPUT);
  pinMode(seg_e,OUTPUT);
  pinMode(seg_f,OUTPUT);
  pinMode(seg_g,OUTPUT);
  pinMode(seg_dp,OUTPUT);
  pinMode(com,OUTPUT);

  
  
}




// func to play buzzer

void playbuzzer()
{
  
  digitalWrite(buzzerPin, HIGH);
  
  }
  
  // detect button state change
  
  void buttontstateChanged()
  {
    // read the pushbutton input pin:
 buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      counter++;
      
    } else {
     
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
  }

 
// print counter number in 7 segment display unit

void printcounterValue()
{
  switch(counter){
    case 1:
    print1();
    break;
    case 2:
    print2();
    break;
    case 3:
    print3();
    break;
    case 4:
    print4();
    break;
    case 5:
    print5();
    break;
    case 6:
    print6();
    break;
    case 7:
    print7();
    break;
    case 8:
    print8();
    break;
    case 9:
    print9();
    break;
    
  }
}

// print whistle number into 7 segment display 
void printwhistleValue()
{
  switch(whistle){
    case 0:
    print0();
    break;
    case 1:
    print1();
    break;
    case 2:
    print2();
    break;
    case 3:
    print3();
    break;
    case 4:
    print4();
    break;
    case 5:
    print5();
    break;
    case 6:
    print6();
    break;
    case 7:
    print7();
    break;
    case 8:
    print8();
    break;
    case 9:
    print9();
    break;
    
  }
}

//func to pre-set desired counter

void setCounter()
{
  start=millis();
  while(elapsed<=50000)    // 50 seconds time is given for setting counter timer
  {
  buttontstateChanged();
  printcounterValue();
  finished=millis();
  elapsed=finished-start;
  }
  
   Counterflag = false;
  }
 
  




void loop() {
  
  //Set counter at beginning of program
  if(Counterflag == true)
  {
  setCounter();
  }
  
  //calling whistle detecting function
 printwhistleValue();
 if(state==HIGH)
 {
   whistle++;
  printwhistleValue();
   delay(20000); // 20 secs minimum gap is given between two whistle sounds.
   state = LOW;                                                                                                                                                                                                                                            ;
 }
  
    
//compares if whistle number is greater than equal to pre-set counter, if yes then kick off buzzer alarm.
if(whistle>=counter)
{
  playbuzzer();
}





 
}
