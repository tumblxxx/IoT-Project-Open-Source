// defines pins numbers
#define stepPinX  2  
#define dirPinX  5 
#define endPin  8

int ang = 0;
//for the delay
int mils = 1;

void setup()
{
  // Sets the two pins as Outputs
  pinMode(stepPinX,OUTPUT); 
  pinMode(dirPinX,OUTPUT);
  pinMode(endPin,OUTPUT);
  Serial.begin(9600);
}

void loop()
{  
  if (Serial.available())
  {
    ang = Serial.parseInt();
    Serial.println("Read " + String(ang) + " degrees");
    rotate(ang); 
  }
}
    void rotate(int angle)
  {

  int numberOfRounds = (int) angle / 1 ;
  for (int i = 0; i < numberOfRounds; i++)
  {
   
    
    digitalWrite(dirPinX,HIGH);
    digitalWrite(stepPinX,LOW);
   

    digitalWrite(dirPinX,LOW);
    digitalWrite(stepPinX,HIGH);
    delay (mils);
    
  }
  // Switch off all phases
  digitalWrite(stepPinX,LOW);
  digitalWrite(dirPinX, LOW);
  digitalWrite(endPin, LOW);

  
  }
