#include <AFMotor.h>
// DC motor on M2
AF_DCMotor motor(2);
void setup() {
  Serial.begin(9600);          
  motor.setSpeed(1000);
  motor.run(RELEASE);
}
void loop()
{int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1);       
if(sensorValue>300)
{
motor.run(FORWARD);
}
else
motor.run(RELEASE);
}
