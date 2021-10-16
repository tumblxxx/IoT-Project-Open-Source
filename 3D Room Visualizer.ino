#include <Servo.h>
Servo servox; Servo servoy;
int servoPosx = 180;
int servoPosy = 180;
int pingPin = 3;
int echoPin = 4;
long duration;
long ping()
{
  // Send out PING))) signal pulse
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(20);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(pingPin, LOW);
  
  //Get duration it takes to receive echo
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
  //Convert duration into distance
  return duration;
}
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
servox.attach(9);
servox.write(130);
servoy.attach(10);
servoy.write(30);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  for(int i=0;i<101;i++){
for(int x=0;x<101;x++){
    int distance = microsecondsToCentimeters(ping());
    Serial.println(distance);
    if (servoPosx <30){
      servoPosx = 130;
    }
    servoPosx -= 1;
    servox.write(servoPosx);
    delay(200);
  }
if (servoPosy > 130){
  servoPosy = 30;
}
servoPosy +=1;
servoy.write(servoPosy);
delay(200);
} 
}
