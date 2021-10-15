


//when covert string into float u will get only 2 digit after float point (.) and for
// that we cant send the value of tuning serially (kp ,ki ,kd) .
// change these value here in arduino sketch .

/* cruise control project .
 * 1)encoder 600ppr 5v two channel A,B
 * 2)motor 24v/150w max speed 3600rpm 
 * 3)motor driver control by pwm .
 * 4)arduino uno .
 * 5)power supply (12v/5a) ... (24v/10a) .
 * 6)gt2 pulley 8mm bore.
 * 7)gt2 pulley 6mm bore .
 * 8)tooth belt 400mm length 6mm width .
 * 9)(x2) fan 5v .
 * 10)(x2)led.
 * 11)(x2)switch on-off 
 * 12)bluetooth module.
 * 13)lcd+i2c .
 * 14)push button .
 * 15) others .
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <PID_v1.h> 
#include <FreqMeasure.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// kalman filter
double K =0;                // kalman gain 
const double R=0.5 ;      // error in measurement

double Eest=0;              // error estimate 
double pre_Eest=0;          // error estimate(t-1)

double est =0;              // estimate 
double pre_est=0;           // estimate(t-1)

double sensor_meg=0;        // from analog pin 

double p1=1; 
const double Q=0.01;     //0.01;          //1e-04;       //1e-04;    // process noise [0.001 slow response]//0.01
/////////////////////////////////////////////////
// kalman filter
 
double K8 =0;                // kalman gain 
const double R8=0.5 ;      // error in measurement [data sheet of lm35 error =0.5]

double Eest8=0;              // error estimate 
double pre_Eest8=0;          // error estimate(t-1)

double est8 =0;              // estimate 
double pre_est8=0;           // estimate(t-1)

double sensor_meg8=0;        // from analog pin 

double p18=1; 
const double Q8=0.0001;       // process noise [0.001 slow response]

// for measure f:-
float rps,rpm ;
int const ppr=600; //pulse per rev.
String data;
String receive;
int sampletime=20;
int mode;
float freq_a;
int Disturbance=0;
int controller=1;
int open_loop_pwm=86;
// sensor :-
double voltage_sensor=12;
double current_sensor=0.55;
double power_sensor=25;

// motor driver :-
int in_A=6; //PWM
//int in_B=5; //PWM
int pwm =50;


// pid parameters:-
double Setpoint,Input,Output;

// from pid matlab tune :-
//double Kp=0.00548333297173391;
//double Ki=0.0423023548974992;
//double Kd=0.000177690701568258;

double Kp=0.00206812507453938;
double Ki=0.0339782106923304; 
double Kd=3.14697363162208e-05;

// define object :-
PID myPID(&Input,&Output,&Setpoint,Kp,Ki,Kd,DIRECT);

// to apps 
String send_to(double s,double i, double e,double o,double ff,double kalman,double v_s,double c_s,double p_s){
String d = (String)s+";"
          +(String)i+";"
          +(String)e+";"
          +(String)o+";"
          +(String)ff+";"
          +(String)kalman+";"
          +(String)v_s+";"
          +(String)c_s+";"
          +(String)p_s
          ;
return d;
d="";
}

void setup() {
  lcd.begin();
  lcd.print("cruise sys");
  delay(2000);
  lcd.clear();
  pinMode(in_A,OUTPUT);
  //pinMode(in_B,OUTPUT);
  pinMode(8,INPUT_PULLUP); // must pull up to 5v to work .
  Serial.begin(9600);
  FreqMeasure.begin();
  myPID.SetMode(AUTOMATIC); //start calculation.
  myPID.SetOutputLimits(0,255);
  myPID.SetSampleTime(sampletime);
  //myPID.SetSampleTime(20); // matlab. 
  Setpoint=2000; // 12v =1800 almost.
  /////////////////////////////////////////////////////////////////////////////////
    uint32_t currentFrequency;
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

}

void right_move(){
  analogWrite(in_A,pwm);
  //digitalWrite(in_B,LOW); 
}

float get_speed(){
      if(FreqMeasure.available()) {
       // if there is data coming do something : 
      float f = FreqMeasure.countToFrequency(FreqMeasure.read());
      freq_a=f;
      
      //convert F to rps :
      rps=f/ppr;
      //convert rps to rpm :
      rpm=rps*60;
      // condition limit :
      if(rpm<0){rpm=0; }
      if(rpm>3600){rpm=3600; }
      
      // effect of disturbance :
      if(rpm>=500){
      rpm=rpm-Disturbance ;
      }
      return rpm;
      }
}

void display_lcd(){
lcd.setCursor(0,0);
lcd.print("S:");
lcd.setCursor(8,0);
lcd.print("I:");
lcd.setCursor(0,1);
lcd.print("E:");
lcd.setCursor(8,1);
lcd.print("o:");

lcd_clear();

// print to lcd after clear :
lcd.setCursor(2,0);
lcd.print((int)Setpoint);
lcd.setCursor(10,0);
lcd.print((int)Input);
lcd.setCursor(2,1);
int e=Setpoint-Input;
lcd.print((int)e);
lcd.setCursor(10,1);
lcd.print((int)Output);
// must have delay above 300ms.

}
void lcd_clear(){

for(int i=2;i<8;i++){ 
  lcd.setCursor(i,0); 
  lcd.print(" "); 
  lcd.setCursor(i,1); 
  lcd.print(" ");
}
for(int i=10;i<16;i++){ 
  lcd.setCursor(i,0); 
  lcd.print(" "); 
  lcd.setCursor(i,1); 
  lcd.print(" "); 
}

}

void parsing (String q){
  String a; // sp
  String b; // sample time
  String c; // mode operation
  String d; // disturbance 

  String a1;
  String b1;
  a=q.substring(0,q.indexOf(";"));
  b=q.substring(q.indexOf(";")+1 , q.indexOf(","));
  c=q.substring(q.indexOf(",")+1 , q.indexOf("&") );
  d=q.substring(q.indexOf("&")+1 , q.indexOf("@") );
  //
  a1=q.substring(q.indexOf("@")+1 , q.indexOf("%") );
  b1=q.substring(q.indexOf("%")+1);

controller=a1.toInt();
typeOfControl(controller);

open_loop_pwm= b1.toInt();

 Disturbance=d.toInt();
 //Output=Output-Disturbance;
 
 Setpoint=a.toInt();
 
 sampletime=b.toInt();
 myPID.SetSampleTime(sampletime);
 
 mode=c.toInt();
 if(mode==1){
  myPID.SetMode(AUTOMATIC);
 }
 else if (mode==0){
  myPID.SetMode(MANUAL);
  Output=open_loop_pwm;
 }

}

void typeOfControl(int x)
{
 
if (x==1){ // pid 
Kp=0.00548333297173391;
Ki=0.0423023548974992; 
Kd=0.000177690701568258;    
  }
  
else if (x==2){ //pi
Kp=0.00284752617249473;
Ki=0.035555779556629; 
Kd=0;    
  }
  
else if (x==3){ //p
Kp=0.0280057653779624;
Ki=0; 
Kd=0;    
  }
  
else if (x==4){ // i
Kp=0;
Ki=0.0187518091153096; 
Kd=0;    
  }
  
else if (x==5){ // oscillator
Kp=0.16;
Ki=0; 
Kd=0;   
  } 
  
myPID.SetTunings(Kp,Ki,Kd);
  
}

void loop()
{ 
unsigned long real_ms=millis();//calculate real time ms ;
// do it later not now.
 
if(Serial.available()>0){
  receive = Serial.readString();
  parsing (receive);

 }
 
right_move();
Input=get_speed();

sensor_meg=Input;
pre_Eest=p1+Q;
K=Eest/(Eest+R);
est=pre_est+K*(sensor_meg-pre_est);
Eest=(1-K)*pre_Eest;

//update kalman
pre_est=est;
p1=Eest;
Input=est;


myPID.Compute();
//Output=Output-Disturbance;
pwm=Output;

//data to serial plotter
/*
Serial.print(Setpoint);
Serial.print(" ");
Serial.print(Input);
Serial.print(" ");
Serial.println(Setpoint-Input);
*/

//show result into lcd screen
display_lcd();

// current 
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

//
sensor_meg8=current_mA;

pre_Eest8=p18+Q8;
//
K8=Eest8/(Eest8+R8);
est8=pre_est8+K8*(sensor_meg8-pre_est8);
Eest8=(1-K8)*pre_Eest8;

//update
pre_est8=est8;
p18=Eest8;

float power_cons=busvoltage*est8;
power_cons=power_cons/1000;

// noise measurement of current sensor :
 float noise =abs( est8 - current_mA );

// send data to c#:
data=send_to(Setpoint,Input,Setpoint-Input,Output,freq_a,noise,busvoltage,est8,power_cons);
Serial.println(data); //println == as string

}
