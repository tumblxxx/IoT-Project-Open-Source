#include <Servo.h>

Servo myservos[4];
const int servosPin[]={6,9,10,11};
const int initialPos[]={80,90,90,0};
const int button1Pin = 12;
const int button2Pin = 8;
const int ledPin = 2;
const int maxLineBuffer=80;

String lineBuffer;

int button1PreviousState=HIGH;
int button2PreviousState=HIGH;

// action state :
// 0 : stop
// 1 : turn left
// 2 : turn right
// 3 : go forward
// 4 : go backward
// 5 : go up
// 6 : go down
// 7 : open
// 8 : close
int state=0;  

void setup() {
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(button1Pin, HIGH);
  digitalWrite(button2Pin, HIGH);
  Serial.begin(11600);
  for (int i=0;i<4;i++) myservos[i].attach(servosPin[i]);  
  setInitialPos();
}

void loop() {
  // Listen to serial port
  while (Serial.available() > 0) {

    char c = Serial.read();
    if (c=='\n' || lineBuffer.length()==maxLineBuffer) {
        lineBuffer.trim();
        int pos=lineBuffer.indexOf(' ');
        if (pos==-1) {  // command with no arg
          if (lineBuffer.compareTo("tl")==0) state=1;       // Turn Left
          else if (lineBuffer.compareTo("tr")==0) state=2;  // Turn Right
          else if (lineBuffer.compareTo("fw")==0) state=3;  // move Forward
          else if (lineBuffer.compareTo("bw")==0) state=4;  // move Backward
          else if (lineBuffer.compareTo("up")==0) state=5;  // move Up
          else if (lineBuffer.compareTo("do")==0) state=6;  // move Down
          else if (lineBuffer.compareTo("op")==0) state=7;  // OPen claw
          else if (lineBuffer.compareTo("cl")==0) state=8;  // CLose claw
          else if (lineBuffer.compareTo("st")==0) state=0;  // STop
          else if (lineBuffer.compareTo("ip")==0) setInitialPos();  // reset to Initial Position
          else if (lineBuffer.compareTo("rp")==0) runProgram();     // Run Program
          else if (lineBuffer.compareTo("he")==0) printHelp();      // HElp
        } else {  // command with one arg
          String cmd=lineBuffer.substring(0,pos);
          int arg=lineBuffer.substring(pos+1).toInt();
          if (cmd.compareTo("s1")==0) {setPos(0,arg);printPos();}       // set Servo 1 position
          else if (cmd.compareTo("s2")==0) {setPos(1,arg);printPos();}  // set Servo 2 position
          else if (cmd.compareTo("s3")==0) {setPos(2,arg);printPos();}  // set Servo 3 position
          else if (cmd.compareTo("s4")==0) {setPos(3,arg);printPos();}  // set Servo 4 position
          else if (cmd.compareTo("li")==0) setLight(arg);               // set LIght [0: OFF, 1: ON]
        }
        lineBuffer="";
    }
    else lineBuffer+=c;
  }
  // Check joysticks
  if (analogRead(A0)<250) {myservos[1].write(myservos[1].read()-1);printPos();}
  if (analogRead(A0)>750) {myservos[1].write(myservos[1].read()+1);printPos();}
  if (analogRead(A1)<250) {myservos[0].write(myservos[0].read()+1);printPos();}
  if (analogRead(A1)>750) {myservos[0].write(myservos[0].read()-1);printPos();}
  if (analogRead(A2)<250) {myservos[2].write(myservos[2].read()-1);printPos();}
  if (analogRead(A2)>750) {myservos[2].write(myservos[2].read()+1);printPos();}
  if (analogRead(A3)<250) {myservos[3].write(myservos[3].read()-1);printPos();}
  if (analogRead(A3)>750) {myservos[3].write(myservos[3].read()+1);printPos();}
  // Check buttons
  if (digitalRead(button1Pin)==LOW && button1PreviousState==HIGH) {
    button1PreviousState=LOW;
    setInitialPos();
  }
  if (digitalRead(button1Pin)==HIGH) button1PreviousState=HIGH;
  if (digitalRead(button2Pin)==LOW && button2PreviousState==HIGH) {
    button2PreviousState=LOW;
    runProgram();
  }
  if (digitalRead(button2Pin)==HIGH) button2PreviousState=HIGH;
  // Check state
  switch(state) {
    case 1:
      myservos[0].write(myservos[0].read()+1);
      printPos();
      break;
    case 2:
      myservos[0].write(myservos[0].read()-1);
      printPos();
      break;      
    case 3:
      myservos[1].write(myservos[1].read()+1);
      printPos();
      break;
    case 4:
      myservos[1].write(myservos[1].read()-1);
      printPos();
      break;      
    case 5:
      myservos[2].write(myservos[2].read()+1);
      printPos();
      break;
    case 6:
      myservos[2].write(myservos[2].read()-1);
      printPos();
      break;      
    case 7:
      myservos[3].write(myservos[3].read()-1);
      printPos();
      break;
    case 8:
      myservos[3].write(myservos[3].read()+1);
      printPos();
      break;      
  }
  // Wait
  delay(250);
}

void setInitialPos() {
  for (int i=0;i<4;i++) setPos(i,initialPos[i]);
  printPos();
}

void setPos(int servo_idx,int targetPos) {
  int currentPos=myservos[servo_idx].read();
  if (targetPos>currentPos) {
    for (int i=currentPos;i<=targetPos;i++) {
      myservos[servo_idx].write(i);
      delay(25);
    }
  }
  if (targetPos<currentPos) {
    for (int i=currentPos;i>=targetPos;i--) {
      myservos[servo_idx].write(i);
      delay(25);
    }
  }
}

void setLight(int x) {
  digitalWrite(ledPin,!x);
}

void runProgram() {
  setPos(0,135);
  setPos(0,35);
  setPos(0,80);
  setPos(1,120);
  setPos(1,70);
  setPos(1,90);
  setPos(2,160);
  setPos(2,70);
  setPos(2,90);
  setPos(3,13);
  delay(500);
  setPos(3,31);
  delay(500);
  setPos(3,13);
  delay(500);
  setPos(3,0);
  printPos();
}

void printPos() {
  for (int i=0;i<4;i++) {
      Serial.print(myservos[i].read());
      Serial.print(" ");
    }
  Serial.println();  
}

void printHelp() {
  Serial.println("tl : turn left");
  Serial.println("tr : turn right"); 
  Serial.println("fw : move forward"); 
  Serial.println("bw : move backward"); 
  Serial.println("up : move up"); 
  Serial.println("do : move down"); 
  Serial.println("op : open claw"); 
  Serial.println("cl : close claw"); 
  Serial.println("st : stop move");
  Serial.println("ip : reset to initial position");
  Serial.println("rp : run program");
  Serial.println("he : print help"); 
  Serial.println("s1 [x] : move servo 1 to position x");
  Serial.println("s2 [x] : move servo 2 to position x");
  Serial.println("s3 [x] : move servo 3 to position x");
  Serial.println("s4 [x] : move servo 4 to position x");
  Serial.println("li [x] : set light [0: OFF, 1: ON]"); 
}
