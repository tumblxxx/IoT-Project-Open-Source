/*
  Overly Complicated one Hour Timer
  A countdown timer controlled by one button that has a range of 0h01 to 1h30 in 7 intervals
  By Ian Cumming https://www.hackster.io/ianabcumming/


  H/W: 3 x LED's, 4 x 220OHM resistors, Piezo Buzzer, Arduino UNO, NPN Transistor, Button.
*/

// LOTS OF DEFINES TO KEEP THINGS EASY TO READ AND SAVE SPACE
#define DEVICE ARDUINO      // ARDUINO or ATTINY85
#define COMMON_PIN_ANODE 1  // 1 = Common Anode; 0 = Common Cathode

#define ATTINY85 1
#define ARDUINO 0

#define SWITCH_OFF_LENGTH 3

#if DEVICE
//ATTINY85 INCLUDES & DEFINES
#include <avr/sleep.h>
#include <avr/interrupt.h>
  #define LED1   0
  #define LED2   1
  #define LED3   2
  #define BUTTON 3
  #define BUZZER 4
#else
  #define LED1   11
  #define LED2   12
  #define LED3   13
  #define BUTTON 2
  #define BUZZER 6
#endif

// PROGRAM STATES
#define CHOOSETIMER 1
#define COUNTDOWN   2
#define BUZZ        3

// BUTTON STATES
#define NO_PRESS 0
#define SHORT_PRESS 1
#define LONG_PRESS 2
#define OFF_PRESS 3

// VARIABLES WOOP!
unsigned long timer = 0;
byte MODE = 2;
byte cdMenu = 1;
int countDown = 45;
int times[] = {0,   60,   300,    900,    1800,   2700,   3600,   5400};

#if DEVICE
void sleep () {                         // with compliments of https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/
  delay(500);                           // BUTTON DEBOUNCE (LONG)

  beep(); delay(200);
  beep(); delay(200);
  beep();

  countDown = times[cdMenu];            // RESET TIMER READY FOR WHEN YOU COME BACK FROM SLEEPING
  MODE = COUNTDOWN;                     // GETTING READY FOR WHEN YOU COME BACK FROM SLEEPING

  GIMSK |= _BV(PCIE);                   // Enable Pin Change Interrupts
  PCMSK |= _BV(BUTTON);                 // Use PB3 as interrupt pin
  ADCSRA &= ~_BV(ADEN);                 // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // replaces above statement

  sleep_enable();                       // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                // Enable interrupts
  sleep_cpu();                          // sleep

  cli();                                // Disable interrupts
  PCMSK &= ~_BV(BUTTON);                // Turn off PB3 as interrupt pin
  sleep_disable();                      // Clear SE bit
  ADCSRA |= _BV(ADEN);                  // ADC on

  sei();                                // Enable interrupts


  beep(); delay(200);
  beep();
}
ISR(PCINT0_vect) {}
#else
void sleep () {

  delay(500); // BUTTON DEBOUNCE (LONG)

  beep(); delay(200);
  beep(); delay(200);
  beep();

  countDown = times[cdMenu];  // RESET TIMER READY FOR WHEN YOU COME BACK FROM SLEEPING
  MODE = COUNTDOWN;           // GETTING READY FOR WHEN YOU COME BACK FROM SLEEPING

  // EMULATES THE SLEEPING WHEN USING THE ARDUIN (IT SAVES NO POWER)
  while (digitalRead(BUTTON));

  beep(); delay(200);
  beep();
}
#endif

// SELF EXPLANITORY
void beep() {
  digitalWrite(BUZZER, HIGH);
  delay(30);
  digitalWrite(BUZZER, LOW);
}

// SETS ALL LED's PINS TO LOW OR HIGH DEPENDING ON THE LED's COMMON PIN CONNECTION
void clearDisplay() {
  byte leds = 0;
#if COMMON_PIN_ANODE
  digitalWrite(LED1, !bitRead(leds, 0));
  digitalWrite(LED2, !bitRead(leds, 1));
  digitalWrite(LED3, !bitRead(leds, 2));
#else
  digitalWrite(LED1, bitRead(leds, 0));
  digitalWrite(LED2, bitRead(leds, 1));
  digitalWrite(LED3, bitRead(leds, 2));
#endif
}
// SHOW THE TIME SELECTED
void displayTime() {
  int leds = 0;
  // GET THE INDEX IN THE ARRAY FROM THE countDown VALUE
  for (int t = 0; t < 8; t++) {
    if (countDown >= times[t]) {
      leds = t;
    }
  }
#if COMMON_PIN_ANODE
  digitalWrite(LED1, !bitRead(leds, 0));
  digitalWrite(LED2, !bitRead(leds, 1));
  digitalWrite(LED3, !bitRead(leds, 2));
#else
  digitalWrite(LED1, bitRead(leds, 0));
  digitalWrite(LED2, bitRead(leds, 1));
  digitalWrite(LED3, bitRead(leds, 2));
#endif
}

// SHOW THE CURRENT COUNT DOWN (ROUNDED UP)
void displayCountDownTime() {
  int leds = 0;
  for (int t = 0; t < 8; t++) {
    if (countDown >= times[t]) {
      leds = t + 1;
    }
  }
#if COMMON_PIN_ANODE
  digitalWrite(LED1, !bitRead(leds, 0));
  digitalWrite(LED2, !bitRead(leds, 1));
  digitalWrite(LED3, !bitRead(leds, 2));
#else
  digitalWrite(LED1, bitRead(leds, 0));
  digitalWrite(LED2, bitRead(leds, 1));
  digitalWrite(LED3, bitRead(leds, 2));
#endif
}

// FINDS OUT THE BUTTON STATES
byte buttonCheck() {
  clearDisplay();

  int butLength = NO_PRESS;
  int mybut = digitalRead(BUTTON);
  if (!mybut) {

    // START TIMING
    unsigned long timerButton = millis() / 10;
    beep();

    // LOOP HERE WHILE THE BUTTON IS DOWN CONTINUING TO TIME
    while (!mybut) {
      mybut = digitalRead(BUTTON);

      // BLINK RANDOMLY WHEN THE TIMER VALUE IS LONGER THAN THE SET VALUE FOR THE SWITCH OFF
      if ((millis() / 10 - timerButton) > (SWITCH_OFF_LENGTH * 100)) {
        digitalWrite(LED1, bitRead(millis() / 100, 0));
        digitalWrite(LED2, bitRead(millis() / 100, 1));
        digitalWrite(LED3, bitRead(millis() / 100, 2));
      }
    }

    clearDisplay();

    // DECIDE WHAT TO RETURN DEPENDING ON HOW LONG THE BUTTON WAS HELD IN
    if ((millis() / 10 - timerButton) > 75) {
      butLength = LONG_PRESS;
    } else {
      butLength = SHORT_PRESS;
    }
    if ((millis() / 10 - timerButton) > (SWITCH_OFF_LENGTH * 100)) {
      butLength = OFF_PRESS;
    }
    delay(50);                    // SORT OF DEBOUNCING THE BUTTON PRESS
  }

  return butLength;
}


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  timer = (millis() / 100);
  clearDisplay();
}

void loop() {
  int button = buttonCheck();

  switch (button) {
    case LONG_PRESS:
      // CYCLE THROUGH THE MODES
      MODE++;
      if (MODE >= BUZZ) {
        MODE = CHOOSETIMER;
      }
      break;
    case OFF_PRESS:
      sleep();
      button = NO_PRESS;                 // FORCE THE BUTTON TO NO_PRESS (I CAN'T REMEMBER WHY THOUGH)
      break;
  }

  switch (MODE) {
    case CHOOSETIMER:
      countDown = times[cdMenu];          // SET THE COUNT DOWN TIME FROM THE ARRAY OF TIMES
      if (button == SHORT_PRESS) {        // CYCLE TIMES
        cdMenu++;
        if (cdMenu == 8) {
          cdMenu = 1; // skip 0 value in the array
        }
        countDown = times[cdMenu];
      }
      displayTime();
      break;
    case COUNTDOWN:
      //EVERY SECCOND FLASH YOUR REMAINING TIME (ROUNDING UP) AND COUNT DOWN
      if (((millis() / 100) - timer) >= 10) {
        timer = (millis() / 100);
        displayCountDownTime();
        delay(50);
        clearDisplay();
        countDown--;
      }

      // WHEN THE COUNTDOWN IS UP BUZZ!!!!!
      if (countDown <= 0) {
        MODE = BUZZ;
      }

      break;
    case BUZZ:
      // BUZZ!
      digitalWrite(BUZZER, HIGH);

      // FLASH!
      digitalWrite(LED1, bitRead(millis() / 100, 0));
      digitalWrite(LED2, bitRead(millis() / 100, 1));
      digitalWrite(LED3, bitRead(millis() / 100, 2));

      // SWITCH OFF WHEN THE BUTON IS PRESSED
      if (button == SHORT_PRESS) {
        digitalWrite(BUZZER, LOW);
        clearDisplay();
        sleep();
        button = NO_PRESS;                 // FORCE THE BUTTON TO NO_PRESS (I CAN'T REMEMBER WHY THOUGH)
      }
      break;
  }
}
