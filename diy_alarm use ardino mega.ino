#include "Time.h"
#include <EEPROM.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>//IR remote library

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

enum LCD_STATE{NORMAL, TIMESET,ALARM};
enum ALARM_STATE{START_AL,HOUR10_AL,HOUR_AL,MIN10_AL,MIN_AL,END_AL};
enum TIMESET_STATE{START,HOUR10,HOUR,MIN10,MIN,SEC10,SEC,YEAR1000,YEAR100,YEAR10,YEAR,MONTH10,MONTH,DAY10,DAY,END};

 //tmElements_t tm;
TimeElements tm;
TimeElements tm_alarm;
time_t t1;
int alarm_id;
int buzzerPin = 5; //Define buzzerPin
int Remot_RECV_PIN = 9; //Pin 9 for IR remote receiver
int addr = 0; // initial EEPROM adress
IRrecv irrecv(Remot_RECV_PIN);// IR remote class
decode_results Remote_addr;// IR key address

void setup() {
  
    lcd.begin(20,4);
    irrecv.enableIRIn();// IR remote receiver enable
    EEPROM.get(addr, t1);
    setTime(t1);
    pinMode(buzzerPin, OUTPUT); //Set buzzerPin as output
    analogWrite(buzzerPin, LOW);
    lcd.clear();
    
}

void loop() {
  static String char_remote = "nothing"; // IR remote character
  static int lcd_state = NORMAL;
  static int alarm_flag = 0;
  int current_hour;
  int current_min;
  int current_sec;
  
  t1 = now();// get current time
  // get the IR remote result
    if (irrecv.decode(&Remote_addr)) {
    char_remote = IR_key(char_remote);    
    irrecv.resume(); // Receive the next value
    }
  // check remote string for lcd state
  if(char_remote == "ch+"){
    lcd_state += 1;
    if(lcd_state >=3){
      lcd_state -= 1;
    }
    char_remote = "nothing";
  }
  else if (char_remote == "ch-"){
    lcd_state -= 1;
    if(lcd_state < 0){
      lcd_state = 0;
    }
    char_remote = "nothing";
  }
  else if(char_remote == "play"){
   // t.stop(alarm_id);
    alarm_flag = 0;
    analogWrite(buzzerPin, 0);
    lcd.setCursor(5,1); 
    lcd.print("                ");//clean the line;
         
  }
  switch(lcd_state){
    case NORMAL: 
         // display lcd state
         lcd.setCursor(5,0); 
         lcd.print("Clock time:");
         
         current_hour = hour(t1);
         current_min = minute(t1);
         current_sec = second(t1);
         if((tm_alarm.Hour == current_hour) && (tm_alarm.Minute == current_min) && (tm_alarm.Second == current_sec)){
           alarm_flag = 1;
           lcd.setCursor(5,1); 
           lcd.print("Time to get up!");
         }
         if (alarm_flag && current_sec%2){analogWrite(buzzerPin, 200); }
         else {analogWrite(buzzerPin, LOW);}
         
         //Display time
         lcd.setCursor(5,2);         
         lcd.print(int(current_hour/10));
         lcd.print(current_hour%10);
         lcd.print(":");
         lcd.print(int(current_min/10));
         lcd.print(current_min%10);
         lcd.print(":");
         lcd.print(int(current_sec/10));
         lcd.print(current_sec%10);
         lcd.print("  ");
         lcd.setCursor(5,3);
         lcd.print(year(t1));
         lcd.print(" ");
         lcd.print(int(month(t1)/10));
         lcd.print(month(t1)%10);
         lcd.print(" ");
         lcd.print(int(day(t1)/10));
         lcd.print(day(t1)%10);
         lcd.print(" ");
         EEPROM.put(addr, t1);break;
    case TIMESET:
         lcd.setCursor(5,0); 
         lcd.print("Setup time:   ");
         time_set_fun(lcd_state, char_remote);
         break;
    case ALARM:
         lcd.setCursor(5,0); 
         lcd.print("Setup alarm:   ");
         alarm_set_fun(lcd_state, char_remote);
         break;
   // default: break; 
  }
  char_remote = "nothing";
}

void alarm_set_fun(int &lcd_state, String char_remote){
  static int alarmset_state = START_AL;
  static int H_prev = 0, M_prev = 0;
  switch(alarmset_state){
    case START_AL:
         tm_alarm.Hour = H_prev;
         tm_alarm.Minute = M_prev;
         alarm_set_display(alarmset_state);
         alarmset_state = HOUR10_AL;
    case HOUR10_AL:
         if(char_remote != "nothing"){
           if(char_remote == "EQ"){ alarmset_state = END_AL;}
           else if(char_remote == "right"){ alarmset_state++;}
           else {
             tm_alarm.Hour = tm_alarm.Hour%10 + (char_remote.toInt()%10)*10;
             alarmset_state = HOUR_AL;
             
           }
         }
         alarm_set_display(alarmset_state);
         break;
    case HOUR_AL:
         if(char_remote != "nothing"){
           if(char_remote == "EQ"){ alarmset_state = END_AL;}
           else if(char_remote == "right"){ alarmset_state++;}
           else {
             tm_alarm.Hour = int(tm_alarm.Hour/10)*10 + char_remote.toInt()%10;
             alarmset_state = MIN10_AL;
             
           }
         }
         alarm_set_display(alarmset_state);
         break;
    case MIN10_AL:
         if(char_remote != "nothing"){
           if(char_remote == "EQ"){ alarmset_state = END_AL;}
           else if(char_remote == "right"){ alarmset_state++;}
           else {
             tm_alarm.Minute = tm_alarm.Minute%10 + (char_remote.toInt()%10)*10;
             alarmset_state = MIN_AL;
             
           }
         }
         alarm_set_display(alarmset_state);
         break;
    case MIN_AL:
         if(char_remote != "nothing"){
           if(char_remote == "EQ"){ alarmset_state = END_AL;}
           else if(char_remote == "right"){ alarmset_state++;}
           else {
             tm_alarm.Minute = int(tm_alarm.Minute/10)*10 + char_remote.toInt()%10;
             alarmset_state = END_AL;
             
           }
         }
         alarm_set_display(alarmset_state);
         break;
    case END_AL:
         tm_alarm.Second = 0;
         H_prev = tm_alarm.Hour;
         M_prev = tm_alarm.Minute;
         lcd_state = NORMAL;
         alarmset_state = START_AL;
         break;
  }//end of switch
}

void alarm_set_display(int alarmset_state){
  lcd.setCursor(5,2);
  lcd.print(int(tm_alarm.Hour/10));
  lcd.print(tm_alarm.Hour%10);
  lcd.print(":");
  lcd.print(int(tm_alarm.Minute/10));
  lcd.print(tm_alarm.Minute%10);
  
  delay(250);
  if (alarmset_state <= 6){
    if(alarmset_state <= 2){lcd.setCursor((4+alarmset_state),2);}
    else if(alarmset_state <= 4){lcd.setCursor((5+alarmset_state),2);}
    lcd.print(" ");
  }
  delay(250);
}

void time_set_fun(int &lcd_state, String char_remote){
    static int timeset_state = START;
    static int year1 = 2018; // for year setup
  switch(timeset_state){
    case START:
         t1 = now();
         tm.Year = year(t1);
         tm.Month = month(t1);
         tm.Day = day(t1);
         tm.Hour = hour(t1);
         tm.Minute = minute(t1);
         tm.Second = second(t1);
         year1 = 2018;
         time_set_display(year1,timeset_state);
         timeset_state = HOUR10; // no break, directly to the HOUR10 state         
    case HOUR10:    
         if(char_remote != "nothing"){
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             tm.Hour = tm.Hour%10 + (char_remote.toInt()%10)*10;
             timeset_state = HOUR;
             
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case HOUR:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             tm.Hour = int(tm.Hour/10)*10 + char_remote.toInt()%10;
             timeset_state = MIN10;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case MIN10:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END; }
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             tm.Minute = tm.Minute%10 + (char_remote.toInt()%10)*10;
             timeset_state = MIN;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case MIN:
         if(char_remote != "nothing"){ 
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             tm.Minute = int(tm.Minute/10)*10 + char_remote.toInt()%10;
             timeset_state = SEC10;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case SEC10:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             tm.Second = tm.Second%10 + (char_remote.toInt()%10)*10;
             timeset_state = SEC;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case SEC:
         if(char_remote != "nothing"){ 
           if(char_remote == "EQ"){timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             tm.Second = int(tm.Second/10)*10 + char_remote.toInt()%10;
             timeset_state = YEAR1000;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case YEAR1000:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             
             year1 = year1%1000 + (char_remote.toInt()%10)*1000;
             //tm.Year = year1 -1970;
             timeset_state = YEAR100;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case YEAR100:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             
             year1 = int(year1/1000)*1000 + year1%100 + (char_remote.toInt()%10)*100;
             //tm.Year = year1 -1970;
             timeset_state = YEAR10;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case YEAR10:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             
             year1 = int(year1/100)*100 + year1%10 + (char_remote.toInt()%10)*10;
             //tm.Year = year1 -1970;
             timeset_state = YEAR;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case YEAR: 
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             
             year1 = int(year1/10)*10 + (char_remote.toInt()%10);
             //tm.Year = year1 -1970;
             timeset_state = MONTH10;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case MONTH10:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {
             
             tm.Month = tm.Month%10 + (char_remote.toInt()%10)*10;             
             timeset_state = MONTH;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case MONTH:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {             
             tm.Month = int(tm.Month/10)*10 + (char_remote.toInt()%10);             
             timeset_state = DAY10;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case DAY10:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {             
             tm.Day = tm.Day%10 + (char_remote.toInt()%10)*10;             
             timeset_state = DAY;
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case DAY:
         if(char_remote != "nothing"){ 
          
           if(char_remote == "EQ"){ timeset_state = END;}
           else if(char_remote == "left"){ timeset_state--;}
           else if(char_remote == "right"){ timeset_state++;}
           else {             
             tm.Day = int(tm.Day/10)*10 + (char_remote.toInt()%10);             
             timeset_state = END;
             
           }
         }
         time_set_display(year1, timeset_state);
         break;
    case END:
         tm.Year = year1 - 1970;
         t1 = makeTime(tm);
         setTime(t1);
         timeset_state = START;
         lcd_state = NORMAL; // back to normal timer display
         //delay(1000);
         break;
 }// end of switch
}// end function

void time_set_display(int year, int timeset_state){
  lcd.setCursor(5,2);
  lcd.print(int(tm.Hour/10));
  lcd.print(tm.Hour%10);
  lcd.print(":");
  lcd.print(int(tm.Minute/10));
  lcd.print(tm.Minute%10);
  lcd.print(":");
  lcd.print(int(tm.Second/10));
  lcd.print(tm.Second%10);
  lcd.setCursor(5,3);
  lcd.print(int(year/1000));
  lcd.print(int((year%1000)/100));
  lcd.print(int((year%100/10)));
  lcd.print(year%10);
  lcd.print(" ");
  lcd.print(int(tm.Month/10));
  lcd.print(tm.Month%10);
  lcd.print(" ");
  lcd.print(int(tm.Day/10));
  lcd.print(tm.Day%10);  
  
  delay(250);
  if (timeset_state <= 6){
    if(timeset_state <= 2){lcd.setCursor((4+timeset_state),2);}
    else if(timeset_state <= 4){lcd.setCursor((5+timeset_state),2);}
    else if(timeset_state <= 6){lcd.setCursor((6+timeset_state),2);}
    lcd.print(" ");
  }
  else {
    if(timeset_state <= 10){ lcd.setCursor((timeset_state-2),3);}
    else if(timeset_state <= 12){ lcd.setCursor((timeset_state-1),3);}
    else if(timeset_state <= 14){ lcd.setCursor((timeset_state),3);}
    lcd.print(" ");
  }
  delay(250);
}

String IR_key(String char_remote){
  String char_rmt, char_prev;
  switch(Remote_addr.value){
    case 0x61D648B7: char_rmt = "switch"; break;// from here is the first remote
    case 0x61D6D827: char_rmt = "up"; break;
    case 0x61D658A7: char_rmt = "down"; break;
    case 0x61D620DF: char_rmt = "left"; break;
    case 0x61D6609F: char_rmt = "right"; break;
    case 0x61D6A05F: char_rmt = "center"; break;
    case 0xC7138F7F: char_rmt = "center"; break;// one remote last buttom
    case 0xFF6897: char_rmt = "0"; break;//from here this is another remote
    case 0xFF30CF: char_rmt = "1"; break;
    case 0xFF18E7: char_rmt = "2"; break;
    case 0xFF7A85: char_rmt = "3"; break;
    case 0xFF10EF: char_rmt = "4"; break;
    case 0xFF38C7: char_rmt = "5"; break;
    case 0xFF5AA5: char_rmt = "6"; break;
    case 0xFF42BD: char_rmt = "7"; break;
    case 0xFF4AB5: char_rmt = "8"; break;
    case 0xFF52AD: char_rmt = "9"; break;
    case 0xFF9867: char_rmt = "100+"; break;
    case 0xFFB04F: char_rmt = "200+"; break;
    case 0xFFE01F: char_rmt = "-"; break;
    case 0xFFA857: char_rmt = "+"; break;
    case 0xFF906F: char_rmt = "EQ"; break;
    case 0xFF22DD: char_rmt = "prev"; break;
    case 0xFF02FD: char_rmt = "next"; break;
    case 0xFFC23D: char_rmt = "play"; break;
    case 0xFFA25D: char_rmt = "ch-"; break;
    case 0xFF629D: char_rmt = "ch"; break;
    case 0xFFE21D: char_rmt = "ch+"; break;// another remote last buttom
    //case 0xFFFFFFFF: char_rmt = char_remote; break;
    default:  char_rmt = char_remote; break; //char_rmt = "nothing";
  }
  delay(200);
  return char_rmt;
}
