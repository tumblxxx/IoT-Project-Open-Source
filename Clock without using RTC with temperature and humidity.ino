// Ahmad Ordikhani Seyedlar

#include <LiquidCrystal.h>
#include <SimpleDHT.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 15);
volatile int sec, minute = 0, hour = 0;
int b_h = 5;
int b_m = 6;
int pinDHT11 = 2;
int b_startstop = 3;
bool startstop = false;

SimpleDHT11 dht11;

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 3);

  lcd.setCursor(0, 0);
  lcd.print("Ahmad Ordikhani");
  lcd.setCursor(0, 2);
  lcd.print("Clk without RTC");
  delay(3000);
  lcd.clear();

  pinMode(b_h, INPUT_PULLUP);
  pinMode(b_m, INPUT_PULLUP);
  pinMode(b_startstop, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(3), buttons, FALLING);


}

void loop() {

  //Setting the time will stop the clock to set the time
  while (startstop == false)
  {
    lcd.setCursor(0, 1);
    lcd.print("SET");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    if(hour<10)
    {
    lcd.print("0");
    lcd.print(hour);
    }
    else
    lcd.print(hour);
    
    lcd.print(":");
    if(minute<10)
    {
      lcd.print("0");
    lcd.print(minute);
    }
    else
    lcd.print(minute);
    lcd.print(":");
    if(sec<10)
    {
    lcd.print("0");  
    lcd.print(sec);
    }
    else
    lcd.print(sec);
    lcd.print(" ");

    if (digitalRead(b_h) == LOW)
    {
      hour++;
      if (hour > 23)
        hour = 0;
    }

    if (digitalRead(b_m) == LOW)
    {
      minute++;
      if (minute > 59)
        minute = 0;
    }

  }

  //Start the clock
  while (startstop == true)
  {


    // noInterrupts();
    // read with raw sample data.
    byte temperature = 0;
    byte humidity = 0;
    byte data[40] = {0};
    if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
      lcd.setCursor(0, 1);
      lcd.print("Read DHT11 failed");
      return;
    }
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print((int)temperature);
    //lcd.print("*C");
    lcd.print(" ");
    lcd.print("Hum.:");
    lcd.print((int)humidity);
    lcd.print("%");
    //Serial.print((int)temperature); Serial.print(" *C, ");
    // Serial.print((int)humidity); Serial.println(" %");

    lcd.setCursor(0, 0);
    //sec=millis()/1000;
    delay(500);
    lcd.print("Time: ");
    if (hour < 10)
    {
      lcd.print("0");
      lcd.print(hour);
    }
    else
    {
      lcd.print(hour);
    }
    lcd.print(":");
    if (minute < 10)
    {
      lcd.print("0");
      lcd.print(minute);
    }
    else
    {
      lcd.print(minute);

    } lcd.print(":");
    if (sec < 10)
    {
      lcd.print("0");
      lcd.print(sec);
    }
    else
    {
      lcd.print(sec);
    }
    lcd.print(" ");
    //lcd.print(startstop);
    sec++;
    if (sec > 59)
    {
      minute++;
      sec = 0;
      //lcd.clear();
    }

    if (minute > 59)
    {
      hour++;
      minute = 0;
      //lcd.clear();
    }
    if (hour > 23)
    {
      hour = 0;
      //lcd.clear();

    }
  }
}

//Start/Stop the clock
void buttons()
{
  lcd.clear();
  startstop = !startstop;


}
