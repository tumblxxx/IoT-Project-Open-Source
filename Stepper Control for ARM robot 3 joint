// defines pins numbers
const int stepPinX = 2; 
const int stepPinY = 3; 
const int stepPinZ = 4; 

const int dirPinX = 5; 
const int dirPinY = 6; 
const int dirPinZ = 7; 

const int pin8 = 8;

int base = 1;
int joint1 = 1;
int joint2 = 0;

void setup()
{
  // Sets the two pins as Outputs
  pinMode(stepPinX,OUTPUT); 
  pinMode(stepPinY,OUTPUT); 
  pinMode(stepPinZ,OUTPUT);
  
  pinMode(dirPinX,OUTPUT);
  pinMode(dirPinY,OUTPUT);
  pinMode(dirPinZ,OUTPUT);
  
  pinMode(pin8,OUTPUT);
}

void loop()
{  
  digitalWrite(pin8,LOW); 
  
  if (base == 1)
  {
    digitalWrite(dirPinX ,HIGH); //Changes the rotations direction
    digitalWrite(dirPinZ ,HIGH);
    
    // Makes 400 pulses for making two full cycle rotation
    for(int x = 1; x < 108; x++) 
    {
      digitalWrite(stepPinX, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinX,LOW);
      delayMicroseconds(1000);

      digitalWrite(stepPinZ, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinZ,LOW);
      delayMicroseconds(1000);
    }
    delay (1000);
    /*digitalWrite(dirPinX,LOW); //Changes the rotations direction
    
    //Makes 400 pulses for making two full cycle rotation
    for(int x = 0; x < 1000; x++) 
    {
      digitalWrite(stepPinX,HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinX,LOW);
      delayMicroseconds(1000);
    }
  }
  
   if (joint1 == 1)
  {
    digitalWrite(dirPinY,HIGH); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 108; x++) 
    {
      digitalWrite(stepPinY, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinY,LOW);
      delayMicroseconds(1000);

      digitalWrite(stepPinY, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinY,LOW);
      delayMicroseconds(1000);
    }
    delay (1000);
    /*digitalWrite(dirPinY,LOW); //Changes the rotations direction
   //Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 120; x++) 
    {
    digitalWrite(stepPinY,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinY,LOW);
    delayMicroseconds(300);
    }
    delay (2000);*/
  }
  
   if (joint2 == 1)
  {
    digitalWrite(dirPinZ,HIGH); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 1000; x++) 
    {
    digitalWrite(stepPinZ,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinZ,LOW);
    delayMicroseconds(1000);
    }
    digitalWrite(dirPinZ,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 1000; x++) 
    {
    digitalWrite(stepPinZ,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinZ,LOW);
    delayMicroseconds(1000);
    }
  }
}
