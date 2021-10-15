#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 8, 9, 1);

const int trigPin = 7; 
const int echoPin = 4;
const int mt_En_Pin1  = 2; 
const int mt_IN1_Pin2  = 3; 
const int mt_IN2_Pin3  = 6;
int buzz = 10;

long duration;
int distance;

void setup()
{
lcd.clear();
lcd.begin(16, 2);
lcd.print("WELCOME");

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(mt_En_Pin1, OUTPUT);
pinMode(mt_IN1_Pin2, OUTPUT);
pinMode(mt_IN2_Pin3, OUTPUT);
Serial.begin(9600);
pinMode(buzz, OUTPUT);
}

void loop() 
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);

distance= duration*0.034/2;
if(distance <= 5)
{
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(mt_En_Pin1, HIGH);
    analogWrite(mt_IN1_Pin2,50);
    analogWrite(mt_IN2_Pin3, 0);
    delay(2000);
    analogWrite(mt_IN1_Pin2, 0);
    analogWrite(mt_IN2_Pin3, 0);
    delay(1000);
    
    tone(buzz, 1000);
    delay(1000);
    tone(buzz, 1000);
    delay(1000);
    noTone(buzz);
    delay(3000);
    
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Please Enter");
    delay(1000);
  
    analogWrite(mt_IN1_Pin2,0);
    analogWrite(mt_IN2_Pin3,50);
    delay(3000);
}

else
{
    digitalWrite(13, LOW);
    digitalWrite(mt_En_Pin1, LOW);
    analogWrite(mt_IN1_Pin2,50);
    analogWrite(mt_IN2_Pin3, 0);    
}

}
