#define Motor1A 2
#define Motor1B 3
#define Motor2A 4
#define Motor2B 5

#define echoPin 10
#define trigPin 11

long tempo = 0;
byte distancia = 0;

void setup() 
{
  
  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor2B, OUTPUT);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() 
{
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  tempo = pulseIn(echoPin, HIGH);

    delay(300);

  distancia = tempo * (0.034/2);
  
  if(distancia >= 10)
  {
    frente();
  }

    else
    
  
  {
    parar();
    delay(500);
    direita();
    delay(1000);
    parar();
  }
  
}

void frente()
{
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2B, LOW);
  
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor2A, HIGH);
}

void re()
{
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor2A, LOW);
  
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2B, HIGH);
}

void parar()
{
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor2A, HIGH);
  
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2B, HIGH); 
}

void direita()
{ 
  digitalWrite(Motor1B, LOW);  
  digitalWrite(Motor2A, LOW);  
  
  digitalWrite(Motor1A, HIGH);  
  digitalWrite(Motor2B, HIGH);
}
