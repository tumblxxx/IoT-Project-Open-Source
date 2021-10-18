#define BLYNK_PRINT Serial          // Blynk 
#include <ESP8266WiFi.h>            // WIFI
#include <BlynkSimpleEsp8266.h>     
#include<Servo.h>                 //Servo SG90
char auth[] = "483e2a27dc3b4fdcb5108b*******";   // token
char ssid[] = "J**** Sidh***";  // SSID WIFI
char pass[] = "190****";   // PASSWORD WIFI
Servo servo;
#define trigPin 13 //Ultrasonic1
#define echoPin 12 //Ultrasonic1
#define trigPin2 5 //Ultrasonic2
#define echoPin2 4 //Ultrasonic2
#define LED 2 //LED WIFI 
#define BLYNK_MAX_SENDBYTES 256 //256 Bytes

void setup() 
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);     //Ultrasonic1
  pinMode(echoPin, INPUT);      //Ultrasonic1
  pinMode(trigPin2, OUTPUT);    //Ultrasonic2
  pinMode(echoPin2, INPUT);     //Ultrasonic2 
  pinMode(LED, OUTPUT);         //LED
  servo.attach(16);       //Servo
  Blynk.begin(auth, ssid, pass); 
  Blynk.email("josie**************@gmail.com", "TrashBin", "Online."); // Test Online Email Sent
  Blynk.notify("Tong Sampah Sudah Online"); //Notify Trash Online
  servo.write(210); // Starting Position Servo
}
void loop() 
{
  Blynk.run();
  
   //Ultrasonic1
  long duration, distance;       
  digitalWrite(trigPin, LOW);        
  delayMicroseconds(2);              
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);           
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Blynk.virtualWrite(V1, distance); //Level
  //Ultrasonic1
                                  
  //Ultrasonic2                                    
  long duration2, distance2;        
  digitalWrite(trigPin2, LOW);        
  delayMicroseconds(2);              
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);           
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  Blynk.virtualWrite(V2, distance2); //Level   
  //Ultrasonic2
 
  // Open Automation
  if (distance >= 30 || distance <= 0)    //Condition when trash open and close automatically
  {
    Serial.println("Out of range");
    servo.write(210);  //menutup
    if (distance2 >= 6 || distance2 <= 0)  //Condition When Trash close reading full or not 
    {
      Serial.print(distance2);
      Serial.println(" cm2");
      digitalWrite(LED, HIGH);
      delay(500);
    }
    else
    {
      digitalWrite(LED, LOW);
      delay(3000);
      Serial.println("FULL");
      Blynk.email("jo********@gmail.com", "Subject: TrashBin", "Full"); //if trash full will sending you email every 15minutes 
      Blynk.notify("Hey, Tong Sampah Penuh Segera Dikosongkan"); //Notify if trash full with blynk
    }
  }
  else 
  {
    Serial.print(distance);
    Serial.println(" cm");
    servo.write(60);      //Open trash
    delay(5000);      //Delay open trash
  }
  delay(500); //Reading hands
}
//Open Automation

  //Open Manually 
  BLYNK_WRITE(V3)
  {
  servo.write(param.asInt());//open
  delay(5000);
  }
  BLYNK_WRITE(V5)
  {
   servo.write(param.asInt());
   delay(5000);
  }
  //Open Manually
