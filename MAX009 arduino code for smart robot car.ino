#include <MsTimer2.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

const int IntPin0=2;
const int IntPin1=3;
const int IntDebounce=50;
const int MotorRDirectionPin =  4; 
const int MotorRSpeedPin =  5; 
const int MotorLDirectionPin =  7; 
const int MotorLSpeedPin =  6;
const int ProxSensor1Pin = 8;
const int ProxSensor2Pin = 9;
const int EchoTriggerPin = 10;
const int EchoPin = 11;
SoftwareSerial softSerial(100,12); // RX, TX
const int ButtonPin = 13;
const int HeadLightsPin = A0;
const int HornPin = A1;
const int LeftLightPin = A2;
const int leftRearLightPin = A3;
const int rightRearLightPin = A4;
const int RightLightPin = A5;
const int maxLineBuffer=80;
double Kc=0.1;
unsigned int speedTarget=150;
const int rotPwm=130;
unsigned int dist=0;

char c;
String lineBuffer;
String pgrmString="";
int inputMode=0; // 0 = normal/interactive, 1 = record program
double pwmInit=130;
double pwmL=pwmInit;
double lasterrorL=0;
double pwmR=pwmInit;
double lasterrorR=0;
int stepCntTarget=0;
boolean useProxSensor=0;

volatile int PIDstate=0;  // 0 = stop, 1 = forward, 2 = backward
volatile unsigned long prevIntTimeL;
volatile unsigned long intPeriodL;
volatile unsigned long prevIntTimeR;
volatile unsigned long intPeriodR;
volatile unsigned int stepCntL=0;
volatile unsigned int stepCntR=0;
unsigned long PIDi=0;

boolean LeftLightState=false;
boolean RightLightState=false;
int SideLightCnt=0;

void setup() {
  pinMode(MotorRDirectionPin, OUTPUT);
  pinMode(MotorRSpeedPin, OUTPUT);
  pinMode(MotorLDirectionPin, OUTPUT);
  pinMode(MotorLSpeedPin, OUTPUT);
  digitalWrite(MotorRDirectionPin,LOW);
  digitalWrite(MotorRSpeedPin,LOW);
  digitalWrite(MotorLDirectionPin,LOW);
  digitalWrite(MotorLSpeedPin,LOW);
  pinMode(ButtonPin, INPUT);
  digitalWrite(ButtonPin, HIGH);
  pinMode(IntPin0,INPUT);
  digitalWrite(IntPin0,HIGH);
  pinMode(IntPin1,INPUT);
  digitalWrite(IntPin1,HIGH);
  attachInterrupt(0, ISR0, RISING);
  attachInterrupt(1, ISR1, RISING);
  pinMode(ProxSensor1Pin,INPUT);
  digitalWrite(ProxSensor1Pin,HIGH);  
  pinMode(ProxSensor2Pin,INPUT);
  digitalWrite(ProxSensor2Pin,HIGH);  
  pinMode(EchoPin,INPUT);
  pinMode(EchoTriggerPin,OUTPUT);
  digitalWrite(EchoTriggerPin,LOW);
  pinMode(HeadLightsPin,OUTPUT);
  digitalWrite(HeadLightsPin,LOW);
  pinMode(HornPin,OUTPUT);
  digitalWrite(HornPin,LOW);
  pinMode(LeftLightPin,OUTPUT);
  digitalWrite(LeftLightPin,LOW);
  pinMode(RightLightPin,OUTPUT);
  digitalWrite(RightLightPin,LOW);
  pinMode(leftRearLightPin,OUTPUT);
  digitalWrite(leftRearLightPin,LOW);
  pinMode(rightRearLightPin,OUTPUT);
  digitalWrite(rightRearLightPin,LOW);
    
  if (digitalRead(ProxSensor1Pin)==LOW || digitalRead(ProxSensor2Pin)==LOW) {
    useProxSensor=false;
    horn(1);
    delay(100);
    horn(0);
  } else {
    useProxSensor=true;
    horn(1);
    delay(100);
    horn(0);
    delay(100);
    horn(1);
    delay(100);
    horn(0);
  }

  Serial.begin(115200);
  // Load program from eeprom
  int i=0;
  c=EEPROM.read(i);
  while(c!=0) {
    pgrmString+=c;
    i++;
    c=EEPROM.read(i);
  }

  softSerial.begin(9600);
  
  // Timer interrupt
  MsTimer2::set(50, timerISR); 
  MsTimer2::start();
}

void loop() {
  while (Serial.available() > 0) {
    c = Serial.read();
    if (c=='\n' || lineBuffer.length()==maxLineBuffer) {
      if (inputMode==1) {
        lineBuffer.trim();
        if (lineBuffer.compareTo("ep")==0) {
          // Save program to eeprom
          for (int i=0;i<pgrmString.length();i++) EEPROM.write(i,pgrmString.charAt(i));
          EEPROM.write(pgrmString.length(),0);
          Serial.println("Program recorded : ");
          Serial.println(pgrmString);
          inputMode=0;
        } else {  
          if (!lineBuffer.endsWith(";") && !lineBuffer.endsWith("]")) lineBuffer+=';';
          pgrmString=pgrmString+lineBuffer;
        }
      } else execPgrm(lineBuffer);
      lineBuffer="";
    }
    else lineBuffer+=c;
  }
  if (digitalRead(ButtonPin)==LOW) execPgrm(pgrmString);
}

void moveForward() {
  PIDstate=1;
  PIDi=0;
  pwmL=pwmInit;
  pwmR=pwmInit;
  digitalWrite(MotorLDirectionPin,LOW);
  analogWrite(MotorLSpeedPin,pwmL);    
  digitalWrite(MotorRDirectionPin,LOW);
  analogWrite(MotorRSpeedPin,pwmR);  
}

void moveBackward() {
  PIDstate=2;
  PIDi=0;
  digitalWrite(MotorLDirectionPin,HIGH);
  analogWrite(MotorLSpeedPin,255-pwmL);
  digitalWrite(MotorRDirectionPin,HIGH);
  analogWrite(MotorRSpeedPin,255-pwmR);
}

void turnRight() { 
  digitalWrite(MotorLDirectionPin,LOW);
  analogWrite(MotorLSpeedPin,rotPwm);
  digitalWrite(MotorRDirectionPin,HIGH);
  analogWrite(MotorRSpeedPin,255-rotPwm);
}

void turnLeft() {
  digitalWrite(MotorRDirectionPin,LOW);
  analogWrite(MotorRSpeedPin,rotPwm);
  digitalWrite(MotorLDirectionPin,HIGH);
  analogWrite(MotorLSpeedPin,255-rotPwm);
}

void stopMove() {
  PIDstate=0;
  digitalWrite(MotorRDirectionPin,LOW);
  digitalWrite(MotorRSpeedPin,LOW);
  digitalWrite(MotorLDirectionPin,LOW);
  digitalWrite(MotorLSpeedPin,LOW);
}

void moveForward(int x) {
  stepCntTarget=x;
  stepCntL=0;
  stepCntR=0;
  PIDi=0;
  moveForward();  
  while(stepCntL<stepCntTarget && (!useProxSensor || (digitalRead(ProxSensor1Pin) && digitalRead(ProxSensor2Pin)))) { 
    digitalWrite(EchoTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(EchoTriggerPin, LOW); 
    dist=pulseIn(EchoPin,HIGH,60000)/58;
    if (dist>0&&dist<20) break;
  }
  stopMove();
}

void moveBackward(int x) {
  stepCntTarget=x;
  stepCntL=0;
  stepCntR=0; 
  PIDi=0;
  moveBackward();
  while(stepCntL<stepCntTarget);  
  stopMove();
}

void turnLeft(int x) {
  stepCntTarget=x;
  stepCntL=0;
  stepCntR=0;   
  turnLeft();
  while(stepCntL<stepCntTarget);  
  stopMove();  
}

void turnRight(int x) {
  stepCntTarget=x;
  stepCntL=0;
  stepCntR=0;   
  turnRight();
  while(stepCntL<stepCntTarget);  
  stopMove();
}

void pause(int x) {
  delay(x);
}

void setkc(int x) {
  Kc=double(x)/1000;
}

void setPwmInit(int x) {
  pwmInit=x;
}

void setspeed(int x) {
  speedTarget=x;
}

void leftLight(int x) {
    if (x==0) {
      LeftLightState=false;
      digitalWrite(LeftLightPin,LOW);
    }
    else if (x==2) {
      LeftLightState=false;
      digitalWrite(LeftLightPin,HIGH);
    }
    else {
        digitalWrite(LeftLightPin,HIGH);
        LeftLightState=true;
        if (!RightLightState) SideLightCnt=0;
    }
}

void rightLight(int x) {
    if (x==0) {
      RightLightState=false;
      digitalWrite(RightLightPin,LOW);
    }
    else if (x==2) {
      RightLightState=false;
      digitalWrite(RightLightPin,HIGH);
    }
    else {
      digitalWrite(RightLightPin,HIGH);
      RightLightState=true;
      if (!LeftLightState) SideLightCnt=0;
    }
    
}

void leftRearLight(int x) {
  if (x==0) digitalWrite(leftRearLightPin,LOW);
    else digitalWrite(leftRearLightPin,HIGH);
}

void rightRearLight(int x) {
  if (x==0) digitalWrite(rightRearLightPin,LOW);
    else digitalWrite(rightRearLightPin,HIGH);
}

void headLight(int x) {
  if (x==0) digitalWrite(HeadLightsPin,LOW);
    else digitalWrite(HeadLightsPin,HIGH);
}

void horn(int x) {
  if (x==0) digitalWrite(HornPin,LOW);
    else digitalWrite(HornPin,HIGH);
}

void help() {
  Serial.println("fw (n) : move forward (by n steps)");
  Serial.println("bw (n) : move backward (by n steps)");
  Serial.println("tl (n) : turn left (by n steps)");
  Serial.println("tr (n) : turn right (by n steps)");
  Serial.println("st : stop move");
  Serial.println("pa [n] : pause of n milliseconds");
  Serial.println("hl [x] : head light [0: OFF, 1: ON]");
  Serial.println("ho [x] : horn [0: OFF, 1: ON]");
  Serial.println("lr [x] : left rear light [0: OFF, 1: ON]");
  Serial.println("rr [x] : right read light [0: OFF, 1: ON]");
  Serial.println("ll [x] : left light [0: OFF, 1: CLIGN, 2:ON]");
  Serial.println("rl [x] : right light [0: OFF, 1: CLIGN, 2:ON]");
  Serial.println("ss [n] : set speed");
  Serial.println("sp : save program");
  Serial.println("ep : end of program");
  Serial.println("rp : run program");
  Serial.println("pp : print program");
  Serial.println("ma.... : forward command ... to MeArm");
  Serial.println("he : help");
}


void processCmd(String str) {
  Serial.print(str);
  Serial.print(" : ");

  int pos=str.indexOf(' ');
  boolean ok=1;
  
  if (str.startsWith("ma")) softSerial.println(str.substring(2)); // Forward command to MeArm
  else {
    if (pos==-1) {  // command with no arg
      if (str.compareTo("fw")==0 || str.compareTo("8")==0) moveForward();
      else if (str.compareTo("bw")==0 || str.compareTo("2")==0) moveBackward();
      else if (str.compareTo("tl")==0 || str.compareTo("4")==0) turnLeft();
      else if (str.compareTo("tr")==0 || str.compareTo("6")==0) turnRight();
      else if (str.compareTo("st")==0 || str.compareTo("5")==0) stopMove();
      else if (str.compareTo("sp")==0) {pgrmString="";inputMode=1;}
      else if (str.compareTo("rp")==0) execPgrm(pgrmString);
      else if (str.compareTo("pp")==0) Serial.println(pgrmString);
      else if (str.compareTo("he")==0) help();
      else ok=0;
    } else {  // command with at least 1 arg
      String cmd=str.substring(0,pos);
      int arg=str.substring(pos+1).toInt();
      if (cmd.compareTo("fw")==0) moveForward(arg);
      else if (cmd.compareTo("bw")==0) moveBackward(arg);
      else if (cmd.compareTo("tl")==0) turnLeft(arg);
      else if (cmd.compareTo("tr")==0) turnRight(arg);
      else if (cmd.compareTo("pa")==0) pause(arg);
      else if (cmd.compareTo("kc")==0) setkc(arg);    
      else if (cmd.compareTo("ss")==0) setspeed(arg);        
      else if (cmd.compareTo("ll")==0) leftLight(arg);        
      else if (cmd.compareTo("rl")==0) rightLight(arg);
      else if (cmd.compareTo("lr")==0) leftRearLight(arg);        
      else if (cmd.compareTo("rr")==0) rightRearLight(arg);
      else if (cmd.compareTo("hl")==0) headLight(arg);
      else if (cmd.compareTo("ho")==0) horn(arg);
      else ok=0;
    }
  }
  if (ok) Serial.println("OK");
    else Serial.println("Error");
  if (inputMode==1) Serial.println("Recording program");
}

void execPgrm(String str) {
  while(str.length()>0) {
    int endpos;
    str.trim();
    if (str.startsWith("repeat")) {
      int startpos=str.indexOf('[');
      int n=str.substring(6,startpos).toInt();
      int level=1;
      endpos=startpos;
      while(endpos<str.length()&&level>0) {
        endpos++;
        if (str.charAt(endpos)=='[') level++;
        if (str.charAt(endpos)==']') level--;
      }
      for (int i=0;i<n;i++) execPgrm(str.substring(startpos+1,endpos));
    } else {
      endpos=str.indexOf(';');
      if (endpos==-1) endpos=str.length();
      processCmd(str.substring(0,endpos));
    }
    if (endpos<str.length()-1) str=str.substring(endpos+1);
      else str="";
  }
}

void ISR0() {
  unsigned long intTime=micros();
  if ((intTime-prevIntTimeR)>IntDebounce && digitalRead(IntPin0)==HIGH) {
    intPeriodR=intTime-prevIntTimeR;
    prevIntTimeR=intTime;
    stepCntR++;
  }
}

void ISR1() {
  unsigned long intTime=micros();
  if ((intTime-prevIntTimeL)>IntDebounce && digitalRead(IntPin1)==HIGH) {
    intPeriodL=intTime-prevIntTimeL;
    prevIntTimeL=intTime;
    stepCntL++;    
  }
}

void timerISR() {
  if (PIDstate>0) {
    double target=speedTarget;
    if (PIDi<8) target=target*PIDi/8;
    double meas=3e6/double(intPeriodL);
    if (meas>1000) meas=0;
    double error=target-meas;
    pwmL=pwmL+Kc*error;
    if (pwmL>255) pwmL=255;
    if (pwmL<0) pwmL=0;
    lasterrorL=error;
    if (PIDstate==2) analogWrite(MotorLSpeedPin,255-pwmL);
      else analogWrite(MotorLSpeedPin,pwmL);
  
    meas=3e6/double(intPeriodR);
    if (meas>1000) meas=0;
    error=target-meas;
    pwmR=pwmR+Kc*error;
    if (pwmR>255) pwmR=255;
    if (pwmR<0) pwmR=0;
    lasterrorR=error;
    if (PIDstate==2) analogWrite(MotorRSpeedPin,255-pwmR);
      else analogWrite(MotorRSpeedPin,pwmR);

    PIDi++;    
  }

  SideLightCnt++;
  if (LeftLightState && (SideLightCnt%10)==0) digitalWrite(LeftLightPin,(SideLightCnt%20)==0);
  if (RightLightState && (SideLightCnt%10)==0) digitalWrite(RightLightPin,(SideLightCnt%20)==0);
}
