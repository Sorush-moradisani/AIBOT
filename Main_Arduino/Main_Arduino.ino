/*
    AIBOT Main Arduino Sketch
    Version 1.1
    Written by Sorush Moradisani
*/
#include <ServoTimer2.h>      //declare ServoTimer2 library to run servo with TMRpcm
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <VarSpeedServo.h> 

ServoTimer2 rhand;
ServoTimer2 lhand;
ServoTimer2 rarm;
ServoTimer2 larm;
ServoTimer2 head;
ServoTimer2 gas;
ServoTimer2 waist;
VarSpeedServo  Rwheel;
VarSpeedServo  Lwheel;

int RED = A4;
int SeqCounter = 0;
int SeqCounter2 = 0;
int VoiceMode = 0;
bool Voice = false;
bool mission = false;

SoftwareSerial mySerial(A0, A1); 

String command, commandS;
bool commandEnd=false, commandEndS=false;
char inByte, inByteS;
int arg = 0, argS = 0;
int intro = 0;
bool isaudio = false;
bool follow = false;
bool found = true;
int light = 0;
bool autoHome = false;
bool fire = false;
int spark = A3;
int X=10, Y=10, I=10, J=10;
bool Rhand = false, Lhand = false;
int MIN_HEAD_ANGLE = 600, MAX_HEAD_ANGLE = 1900;
int MIN_WAIST_ANGLE = 700, MAX_WAIST_ANGLE = 1900;
int MIN_LHAND_ANGLE = 544, MAX_LHAND_ANGLE = 2400;
int MIN_RHAND_ANGLE = 544, MAX_RHAND_ANGLE = 2400;
int MIN_RARM_ANGLE = 800, MAX_RARM_ANGLE = 2400;
int MIN_LARM_ANGLE = 800, MAX_LARM_ANGLE = 2400;
int MIN_FIRESERVO_ANGLE = 700, MAX_FIRESERVO_ANGLE = 1900;
/*Rwheel Min = 0(forward), Middle = 1500(Stop), Max = 2400(Backward)*/
int VSPEED = 5 , HSPEED = 10 , MSPEED = 50, MACCEL = 125;
int MAX_VSPEED = 10 , MAX_HSPEED = 20 , MAX_MSPEED = 255, MSTOP = 1500, MSTOPdeg = 90;

int rand_min = 0, rand_max = 8;

int figurcounter1 = 0, figurcounter2 = 0;
void setup() 
{
  Serial.begin(115200);
  mySerial.begin(9600);

  Rwheel.attach(5);
  delay(100);
  Rwheel.write(MSTOP);
  delay(300);

  Lwheel.attach(6);
  delay(100);
  Lwheel.write(MSTOP);
  delay(300);

  gas.attach(7);
  delay(100);
  gas.write(1300);
  delay(300);

  head.attach(4); 
  delay(100);
  head.write(1200);
  delay(300);

  lhand.attach(3);
  delay(100);
  lhand.write(2400);
  delay(300);

  rhand.attach(2);
  delay(100);
  rhand.write(544);
  delay(300);

  larm.attach(8);
  delay(100);
  larm.write(MAX_LARM_ANGLE);
  delay(300);

  rarm.attach(A2);
  delay(100);
  rarm.write(MIN_RARM_ANGLE);
  delay(300);

  waist.attach(A5);
  delay(100);
  waist.write(1200);
  delay(300);

  
  //tmr.quality(1);

  pinMode(RED, OUTPUT);
  pinMode(spark, OUTPUT);
  digitalWrite(spark, HIGH);


  //lhand.write(2200);


 VoiceMode = EEPROM.read(0);
//  if(VoiceMode == 0){
  
//  }else if(VoiceMode == 1){
//   autoHome = true;
//   // Serial.println("log");
//   // Rwheel.write(1800);
//   // Lwheel.write(1800);
//   // delay(500);
//   // Rwheel.write(1200);
//   // Lwheel.write(1200);
//   // delay(500);
//   // Rwheel.write(0);
//   // Lwheel.write(0);
//   for (int pos = 0; pos <= 1800; pos += 1) { 
//     rhand.write(pos+544);
//     lhand.write(2400-pos);
//     //if(pos<1000){lhand.write(2400-pos);}
//     delay(1);                       
//   }
//   for (int pos = 1800; pos >= 0; pos -= 1) { 
//     rhand.write(pos+544); 
//     lhand.write(2400-pos);
//     //if(pos<1000){lhand.write(2400-pos);}
//     delay(1);                      
//   }
//   for (int pos = 1200; pos <= 2000; pos += 1) {
//     head.write(pos);
//     delay(2);                      
//   for (int pos = 2000; pos >= 1200; pos -= 1) { 
//     head.write(pos); 
//     delay(2);                       
//   }
//  } 

//  }

}

void loop() 
{
  if (Serial.available()>0) {

      if(commandEndS){
      commandS="";
      commandEndS=false;
    }
      inByteS = Serial.read();
      if (inByteS != '\n') {
        if (inByteS == '=') {
          argS = Serial.parseInt();
          //Serial.println(arg);      
        } else {
          commandS += inByteS;
        }
      }else{
        commandEndS=true;
        if(compareString(commandS, "audio")){
          isaudio = argS;
        }
    }
  }
  if (mySerial.available() > 0) {

      if(commandEnd){
      command="";
      commandEnd=false;
    }
      inByte = mySerial.read();
      if (inByte != '\n') {
        if (inByte == '=') {
          arg = mySerial.parseInt();
          //Serial.println(arg);      
        } else {
          command += inByte;
        }
      }else{
        commandEnd=true;
        Serial.print(command);
        Serial.print("=");
        Serial.println(arg);
        execute(command, arg);
      }
  }
    // if(head.read()>=MAX_HEAD_ANGLE)head.write(MAX_HEAD_ANGLE);
    // if(head.read()<=MIN_HEAD_ANGLE)head.write(MIN_HEAD_ANGLE);
    // if(waist.read()>=MAX_WAIST_ANGLE)waist.write(MAX_WAIST_ANGLE);
    // if(waist.read()<=MIN_WAIST_ANGLE)waist.write(MIN_WAIST_ANGLE);
    // if(rhand.read()>=MAX_RHAND_ANGLE)rhand.write(MAX_RHAND_ANGLE);
    // if(rhand.read()<=MIN_RHAND_ANGLE)rhand.write(MIN_RHAND_ANGLE);
    // if(rarm.read()>=MAX_RARM_ANGLE)rarm.write(MAX_RARM_ANGLE);
    // if(rarm.read()<=MIN_RARM_ANGLE)rarm.write(MIN_RARM_ANGLE);

  if(Rhand){
    delay(8);
    int rhandpos = rhand.read();
    int rarmpos = rarm.read();
    if(J<8 && I>8 && I<13){//FORWARD
      if(rhandpos >=MIN_RHAND_ANGLE)rhand.write(rhandpos+HSPEED);
    }else if(J>13 && I>8 && I<13){//BACKWARD
      if(rhandpos <=MAX_RHAND_ANGLE)rhand.write(rhandpos-HSPEED);
    }else if(J<8 && I>13){//FORWARD RIGHT
      if(rhandpos >=MIN_RHAND_ANGLE)rhand.write(rhandpos+HSPEED);
      if(rarmpos <=MAX_RARM_ANGLE)rarm.write(rarmpos+HSPEED);
    }else if(J<8 && I<8){//FORWARD LEFT
      if(rhandpos >=MIN_RHAND_ANGLE)rhand.write(rhandpos+HSPEED);
      if(rarmpos >=MIN_RARM_ANGLE)rarm.write(rarmpos-HSPEED);
    }else if(J>8 && J<13 && I>13){//ROTATE RIGHT
      if(rarmpos <=MAX_RARM_ANGLE)rarm.write(rarmpos+HSPEED);
    }else if(J>8 && J<13 && I<8){//ROTATE LEFT
      if(rarmpos >=MIN_RARM_ANGLE)rarm.write(rarmpos-HSPEED);
    }else if(J>13 && I>13){//BACKWARD RIGHT
      if(rhandpos <=MAX_RHAND_ANGLE)rhand.write(rhandpos-HSPEED);
      if(rarmpos <=MAX_RARM_ANGLE)rarm.write(rarmpos+HSPEED);
    }else if(J>13 && I<8){//BACKWARD LEFT
      if(rhandpos <=MAX_RHAND_ANGLE)rhand.write(rhandpos-HSPEED);
      if(rarmpos >=MIN_RARM_ANGLE)rarm.write(rarmpos-HSPEED);
    }
  }

  if(!Rhand){
    if(J<8 && I>8 && I<13){//FORWARD
      Rwheel.write(MSTOPdeg-MSPEED, MACCEL);
      Lwheel.write(MSTOPdeg+MSPEED, MACCEL);
    }else if(J>13 && I>8 && I<13){//BACKWARD
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);
      Lwheel.write(MSTOPdeg-MSPEED,MACCEL);
    }else if(J<8 && I>13){//FORWARD RIGHT
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOPdeg+MSPEED,MACCEL);
    }else if(J<8 && I<8){//FORWARD LEFT
      Rwheel.write(MSTOPdeg-MSPEED,MACCEL);
      Lwheel.write(MSTOP);
    }else if(J>8 && J<13 && I>13){//ROTATE RIGHT
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);
      Lwheel.write(MSTOPdeg+MSPEED,MACCEL);
    }else if(J>8 && J<13 && I<8){//ROTATE LEFT
      Rwheel.write(MSTOPdeg-MSPEED,MACCEL);
      Lwheel.write(MSTOPdeg-MSPEED,MACCEL);
    }else if(J>13 && I>13){//BACKWARD RIGHT
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);
      Lwheel.write(MSTOP);
    }else if(J>13 && I<8){//BACKWARD LEFT
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOPdeg-MSPEED,MACCEL);
    }
  }

    // if(lhand.read()>=MAX_LHAND_ANGLE)lhand.write(MAX_LHAND_ANGLE);
    // if(lhand.read()<=MIN_LHAND_ANGLE)lhand.write(MIN_LHAND_ANGLE);
    // if(larm.read()>=MAX_LARM_ANGLE)larm.write(MAX_LARM_ANGLE);
    // if(larm.read()<=MIN_LARM_ANGLE)larm.write(MIN_LARM_ANGLE);
    
  if(Lhand){
    delay(20);
    int lhandpos = lhand.read();
    int larmpos = larm.read();
    if(Y<8 && X>8 && X<13){//FORWARD
      if(lhandpos >=MIN_LHAND_ANGLE)lhand.write(lhandpos-HSPEED);
    }else if(Y>13 && X>8 && X<13){//BACKWARD
      if(lhandpos <=MAX_LHAND_ANGLE)lhand.write(lhandpos+HSPEED);
    }else if(Y<8 && X>13){//FORWARD RIGHT
      if(lhandpos >=MIN_LHAND_ANGLE)lhand.write(lhandpos-HSPEED);
      if(larmpos <=MAX_LARM_ANGLE)larm.write(larmpos+HSPEED);
    }else if(Y<8 && X<8){//FORWARD LEFT
      if(lhandpos >=MIN_LHAND_ANGLE)lhand.write(lhandpos-HSPEED);
      if(larmpos >=MIN_LARM_ANGLE)larm.write(larmpos-HSPEED);
    }else if(Y>8 && Y<13 && X>13){//ROTATE RIGHT
      if(larmpos <=MAX_LARM_ANGLE)larm.write(larmpos+HSPEED);
    }else if(Y>8 && Y<13 && X<8){//ROTATE LEFT
      if(larmpos >=MIN_LARM_ANGLE)larm.write(larmpos-HSPEED);
    }else if(Y>13 && X>13){//BACKWARD RIGHT
      if(lhandpos <=MAX_LHAND_ANGLE)lhand.write(lhandpos+HSPEED);
      if(larmpos <=MAX_LARM_ANGLE)larm.write(larmpos+HSPEED);
    }else if(Y>13 && X<8){//BACKWARD LEFT
      if(lhandpos <=MAX_LHAND_ANGLE)lhand.write(lhandpos+HSPEED);
      if(larmpos >=MIN_LARM_ANGLE)larm.write(larmpos-HSPEED);
    }
  }

  if(!Lhand){
    delay(20);
    int panpos = waist.read();
    int tiltpos = head.read();
    if(Y<8 && X>8 && X<13){//FORWARD
      if(tiltpos >=MIN_HEAD_ANGLE)head.write(tiltpos+VSPEED);
    }else if(Y>13 && X>8 && X<13){//BACKWARD
      if(tiltpos <=MAX_HEAD_ANGLE)head.write(tiltpos-VSPEED);
    }else if(Y<8 && X>13){//FORWARD RIGHT
      if(tiltpos >=MIN_HEAD_ANGLE)head.write(tiltpos+VSPEED);
      if(panpos <=MAX_WAIST_ANGLE)waist.write(panpos-VSPEED);
    }else if(Y<8 && X<8){//FORWARD LEFT
      if(tiltpos >=MIN_HEAD_ANGLE)head.write(tiltpos+VSPEED);
      if(panpos >=MIN_WAIST_ANGLE)waist.write(panpos+VSPEED);
    }else if(Y>8 && Y<13 && X>13){//ROTATE RIGHT
      if(panpos <=MAX_WAIST_ANGLE)waist.write(panpos-VSPEED);
    }else if(Y>8 && Y<13 && X<8){//ROTATE LEFT
      if(panpos >=MIN_WAIST_ANGLE)waist.write(panpos+VSPEED);
    }else if(Y>13 && X>13){//BACKWARD RIGHT
      if(tiltpos <=MAX_HEAD_ANGLE)head.write(tiltpos-VSPEED);
      if(panpos <=MAX_WAIST_ANGLE)waist.write(panpos-VSPEED);
    }else if(Y>13 && X<8){//BACKWARD LEFT
      if(tiltpos <=MAX_HEAD_ANGLE)head.write(tiltpos-VSPEED);
      if(panpos >=MIN_WAIST_ANGLE)waist.write(panpos+VSPEED);
    }
  }

  
  if(!isaudio && autoHome){
    if(rhand.read()>544){
      for (int pos = rhand.read(); pos >= 544; pos -= 2) { 
      rhand.write(pos);              
      delay(2);                       
      }
    }
    if(rarm.read()>MIN_RARM_ANGLE){
      for (int pos = rarm.read(); pos >= 544; pos -= 2) { 
      rarm.write(pos);              
      delay(2);                       
      }
    }
    if(rarm.read()<MAX_LARM_ANGLE){
      for (int pos = larm.read(); pos <= MAX_LARM_ANGLE; pos += 2) { 
      larm.write(pos);              
      delay(2);                       
      }
    }
    if(lhand.read()<2400){
      for (int pos = lhand.read(); pos <= 2400; pos += 2) {
      lhand.write(pos);              
      delay(2);                    
      }
    }
    if(waist.read()>1200){
      for (int pos = waist.read(); pos >= 1200; pos -= 2) { 
      waist.write(pos);            
      delay(2);                     
      }
    }else if(waist.read()<1200){
      for (int pos = waist.read(); pos <= 1200; pos += 2) { 
      waist.write(pos);              
      delay(2);                      
      }
    }
    if(head.read()>1250){
      head.write(1200);
    }
    Serial.println("rgb-off");
    autoHome = false;
    if(VoiceMode ==2 && mission){
      //Serial.println("mission.wav");
      mission = false;
      //delay(1500);
    }
    Rwheel.write(MSTOP);
    Lwheel.write(MSTOP);
    //Serial.println("stopaudio");

  }

}

void execute(String command, int arg){/////////////////////////////////////////////////////////////////////////
  if(compareString(command, "sorush")){
    autoHome = true;
    if(!found){
      Serial.println("aha");
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      delay(3000);
      found = true;
    }
    Serial.println("green");
    Serial.println("hidad");
    for (int pos = 544; pos <= 2200; pos += 3) {
    rhand.write(pos);          
    delay(2);                      
  }
      delay(3000);
  command="";
  }else if(compareString(command, "vspeed")){//////////////////////////////////////////////////////////////////////////
    VSPEED = map(arg, 10, 50, 1, MAX_VSPEED);
    Serial.print("vspeed=");
    Serial.println(VSPEED);
  }else if(compareString(command, "hspeed")){///////////////////////////////////////////////////////////////////////////
    HSPEED = map(arg, 10, 50, 1, MAX_HSPEED);
    // Serial.print("hspeed=");
    // Serial.println(arg);
    command="";
  }else if(compareString(command, "mspeed")){////////////////////////////////////////////////////////////////////////////
    MSPEED = map(arg, 10, 50, 0, MAX_MSPEED);
    // Serial.print("mspeed=");
    // Serial.println(MSPEED);
    command="";
  }else if(compareString(command, "maccel")){////////////////////////////////////////////////////////////////////////////
    MACCEL = map(arg, 10, 50, 0, 255);
    // Serial.print("mspeed=");
    // Serial.println(MSPEED);
    command="";
  }else if(compareString(command, "greet")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("green");
      Serial.println("hi");
      delay(3000);

      for (int pos = 544; pos <= 1400; pos += 3) { 
    rhand.write(pos);           
    delay(2);                     
  }
    Serial.println("buiru");
    Serial.println("green");
    delay(1000);
      for (int pos = 1400; pos >= 544; pos -= 2) { 
    rhand.write(pos);              
    delay(2);                       
  }
      delay(3000);
command="";
    }else if(compareString(command, "history")){///////////////////////////////////////////////////////////////////////////
      Serial.println("past");
      command="";
    }else if(compareString(command, "hey")){///////////////////////////////////////////////////////////////////////////
      Serial.println("hey");
      command="";
    }else if(compareString(command, "nocmd")){///////////////////////////////////////////////////////////////////////////
      //Serial.println("history.wav");
      command="";
    }else if(compareString(command, "r-rotate")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);
      Lwheel.write(MSTOPdeg+MSPEED,MACCEL);
      command="";
    }else if(compareString(command, "l-rotate")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOPdeg-MSPEED,MACCEL);
      Lwheel.write(MSTOPdeg-MSPEED,MACCEL);
      command="";
    }else if(compareString(command, "f-forward")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOPdeg-MSPEED, MACCEL);
      Lwheel.write(MSTOPdeg+MSPEED, MACCEL);
      if(!isaudio && VoiceMode == 2){
        if(Voice){
          Serial.println("waypoint");
          Voice = false;
        }else{
          Serial.println("nav");
          Voice = true;
        }
        
      }
      command="";
    }else if(compareString(command, "rhand")){///////////////////////////////////////////////////////////////////////////
      if(Rhand){
        Rhand = false;
      }else{
        Rhand = true;
      }
      command="";
    }else if(compareString(command, "lhand")){///////////////////////////////////////////////////////////////////////////
      if(Lhand){
        Lhand = false;
      }else{
        Lhand = true;
      }
      command="";
    }else if(compareString(command, "x")){///////////////////////////////////////////////////////////////////////////
      X = arg;
      Serial.print("x=");
      Serial.println(X);
      command="";
    }else if(compareString(command, "y")){///////////////////////////////////////////////////////////////////////////
      Y = arg;
      Serial.print("y=");
      Serial.println(Y);
      command="";
    }else if(compareString(command, "i")){///////////////////////////////////////////////////////////////////////////
      I=arg;
      // Serial.print(command);
      // Serial.print("i=");
      // Serial.println(I);
      command="";
    }else if(compareString(command, "j")){///////////////////////////////////////////////////////////////////////////
      // Serial.print(command);
      // Serial.print("j=");
      // Serial.println(J);
      J=arg;
      command="";
    }else if(compareString(command, "f-right")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOPdeg+MSPEED,MACCEL);
      command="";
    }else if(compareString(command, "f-left")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOPdeg-MSPEED,MACCEL);
      Lwheel.write(MSTOP);
      command="";
    }else if(compareString(command, "b-backward")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);
      Lwheel.write(MSTOPdeg-MSPEED,MACCEL);
      command="";
    }else if(compareString(command, "b-left")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOPdeg-MSPEED,MACCEL);
      command="";
    }else if(compareString(command, "b-right")){///////////////////////////////////////////////////////////////////////////
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);
      Lwheel.write(MSTOP);
      command="";
    }else if(compareString(command, "stop")){///////////////////////////////////////////////////////////////////////////
      found = true;
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      I=10;
      J=10;
      X=10;
      Y=10;
      //Serial.println("stop");
      //command="";
    }else if(compareString(command, "lstop")){///////////////////////////////////////////////////////////////////////////
      
      
      X=10;
      Y=10;
      //Serial.println("stop");
      //command="";
    }else if(compareString(command, "rstop")){///////////////////////////////////////////////////////////////////////////
      
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      I=10;
      J=10;
      //Serial.println("stop");
      //command="";
    }else if(compareString(command, "introduce")){//////////////////////////////////////////////////////////////////////
      autoHome = true;
      if(intro==0){
      Serial.println("aibot");
      Serial.println("green");
      for (int pos = 544; pos <= 2200; pos += 3) {
        rhand.write(pos);          
        delay(2);                      
      }
      delay(3000);
      intro++;
    }else{
      Serial.println("aibot2");
      for (int pos = MIN_RARM_ANGLE; pos <= MAX_RARM_ANGLE; pos += 2) {
        rarm.write(pos); 
        larm.write(2200-pos);        
        delay(2);                      
      }
      head.write(800); 
      for (int pos = 1200; pos <= 1400; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(500);
      for (int pos = 1200; pos >= 1000; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(500);
      for (int pos = 1000; pos <= 1200; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      head.write(1200);
      for (int pos = MAX_RARM_ANGLE; pos >= MIN_RARM_ANGLE; pos -= 2) {
        rarm.write(pos); 
        larm.write(2200+pos);        
        delay(2);                      
      }
      delay(10000); 
      head.write(1400); 
      for (int pos = 1200; pos <= 1400; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(500);
      for (int pos = 1400; pos >= 1000; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(500);
      for (int pos = 1000; pos <= 1200; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      head.write(1200);
      delay(3000);
      for (int pos = 1200; pos <= 1600; pos += 1) {
        head.write(pos);
        delay(2);                      
      }
      delay(3000); 
      for (int pos = 1600; pos >= 1200; pos -= 1) { // in steps of 1 degree
        head.write(pos); 
        delay(2);                       
      }
      delay(2000);
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
        rhand.write(600);
        delay(300);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
        rhand.write(600);
        delay(300);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
        rhand.write(600);
        delay(300);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
        delay(1100);
      for (int pos = MIN_RARM_ANGLE; pos <= MAX_RARM_ANGLE; pos += 1) {
        rarm.write(pos);
        delay(2);                      
      }
      for (int pos = 1200; pos >= 700; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(500);
      for (int pos = 700; pos <= 1200; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      for (int pos = MAX_RARM_ANGLE; pos >= MIN_RARM_ANGLE; pos -= 1) { // in steps of 1 degree
        rarm.write(pos); 
        delay(2);                       
      }
      delay(100);
      for (int pos = 1200; pos >= 700; pos -= 1) { // in steps of 1 degree
        head.write(pos); 
        delay(2);                       
      }
      delay(1900);
      for (int pos = 700; pos <= 1200; pos += 1) {
        head.write(pos);
        delay(2);                      
      }
      delay(6000);/********************************************/
      for (int pos = 544; pos <= 1600; pos += 3) { 
        rhand.write(pos);            
        delay(2);                   
      }
      for (int pos = 2200; pos >= 1300; pos -= 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      rarm.write(2200);
      larm.write(1250);
      delay(3000);
      for (int pos = 1300; pos <= 2200; pos += 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      for (int pos = 1600; pos >= 544; pos -= 3) { 
        rhand.write(pos);            
        delay(2);                   
      }
      rarm.write(1250);
      larm.write(2200);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg+MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg-MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg+MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg-MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg+MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg-MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg+MSPEED,150);
      Lwheel.write(MSTOPdeg+MSPEED,150);
      delay(300);
      Rwheel.write(MSTOPdeg-MSPEED,150);
      Lwheel.write(MSTOPdeg-MSPEED,150);
      delay(300);
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      delay(2000);
      for (int pos = 1200; pos <= 1900; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(100);
      for (int pos = 1900; pos >= 700; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(100);
      for (int pos = 700; pos <= 1200; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      for (int pos = 1200; pos >= 700; pos -= 1) { // in steps of 1 degree
        head.write(pos); 
        delay(2);                       
      }
      delay(1000);
      for (int pos = 700; pos <= 1200; pos += 1) {
        head.write(pos);
        delay(2);                      
      }
      delay(6000); 
      for (int pos = 1200; pos <= 1900; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      head.write(800);
      delay(100);
      for (int pos = 1900; pos >= 700; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(100);
      for (int pos = 700; pos <= 1200; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      head.write(1200);
      delay(8000);
      for (int pos = 2200; pos >= 1300; pos -= 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      delay(1000);
      digitalWrite(spark, LOW);
      delay(500);
      digitalWrite(spark, HIGH);
      delay(300);
      digitalWrite(spark, LOW);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      delay(2000);
      Serial.println("blue");
      delay(6000);
      for (int pos = 1300; pos <= 2200; pos += 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      intro=0;
    }
      
      command="";
    }else if(compareString(command, "tilt")){///////////////////////////////////////////////////////////////////////////
      int i = map(arg, 0, 100, 544, 2000);
      head.write(i);
      command="";
    }else if(compareString(command, "autopan") && follow){//////////////////////////////////////////////////////////////
      //int i = map(arg, 0, 320, 800, 1400);
      // if(arg<170 && arg>150){
      // analogWrite(5, 254);
      // analogWrite(6, 22);
      // }else if(arg>=170){
      //   analogWrite(5, 0);
      //   analogWrite(6, 22);
      // }else if(arg<=150){
      //   analogWrite(5, 254);
      //   analogWrite(6, 188);
      // }
      command="";
    }else if(compareString(command, "autotilt") && follow){///////////////////////////////////////////////////////////////////////////
      if(arg>140){
      int i = map(arg, 240, 140, 0, 100);
      head.write(head.read()-i);
    }else if(arg<100){
      int j = map(arg, 100, 0, 0, 100);
      head.write(head.read()+j);
    }
      command="";
    }else if(compareString(command, "follow")){///////////////////////////////////////////////////////////////////////////
      follow = true;
      command="";
    }else if(compareString(command, "nofollow")){///////////////////////////////////////////////////////////////////////////
      follow = false;
      command="";
    }else if(compareString(command, "find")){///////////////////////////////////////////////////////////////////////////
      delay(1000);
      Serial.println("kojai");
      Rwheel.write(MSTOPdeg+20,MACCEL);
      Lwheel.write(MSTOPdeg+20,MACCEL);
      found = false;
      command="";
    }else if(compareString(command, "pan")){///////////////////////////////////////////////////////////////////////////
      int i = map(arg, 0, 100, 544, 2400);
      //waist.write(i);
      Serial.println("pan"); 
      command="";
    }else if(compareString(command, "lr-rhand")){///////////////////////////////////////////////////////////////////////////
      int i = map(arg, 0, 100, 1250, 2300);
      rarm.write(i);
      command="";
    }else if(compareString(command, "ud-rhand")){///////////////////////////////////////////////////////////////////////////
      int i = map(arg, 0, 100, 544, 2400);
      rhand.write(i);
      command="";
    }else if(compareString(command, "lr-lhand")){///////////////////////////////////////////////////////////////////////////
      int i = map(arg, 0, 100, 544, 2400);
      larm.write(i);
      command="";
    }else if(compareString(command, "ud-lhand")){///////////////////////////////////////////////////////////////////////////
      int i = map(arg, 0, 100, 2400, 544);
      lhand.write(i);
      command="";
    }else if(compareString(command, "fire")){///////////////////////////////////////////////////////////////////////////

      delay(100);
      if(fire){
          gas.write(1300);
          fire = false;
        }else{
          digitalWrite(spark, LOW);
          gas.write(700);
          delay(500);
          digitalWrite(spark, HIGH);
          fire = true;
        }
      command="";
    }else if(compareString(command, "torch")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
          
          for (int pos = lhand.read(); pos >= 1000; pos -= 1) { 
            lhand.write(pos);            
            delay(2);                   
          }
          head.write(1500);
          digitalWrite(spark, LOW);
          gas.write(700);
          delay(500);
          digitalWrite(spark, HIGH);
          Serial.println("buiru");
          delay(10000);
          gas.write(1300);

      command="";
    }else if(compareString(command, "liftme")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      
      head.write(800); 
      delay(1000);
      for (int pos = 1200; pos <= 1700; pos += 2) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(1000);
      for (int pos = 1700; pos >= 700; pos -= 2) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(1000);
      for (int pos = 700; pos <= 1200; pos += 2) {
        waist.write(pos);       
        delay(2);                      
      }
      Serial.println("liftme");
      head.write(1800);
      delay(1000);
      for (int pos = MIN_RARM_ANGLE; pos <= 2400; pos += 2) {
        rhand.write(pos); 
        lhand.write(3300-pos);  
        rarm.write(pos); 
        larm.write(2600-pos);      
        delay(2);                      
      }
      for(int i = 0; i<5; i++){
          rhand.write(2400); 
          lhand.write(1100);        
          delay(200);  
          rhand.write(2000); 
          lhand.write(700);        
          delay(200);        
      }
      delay(1000);
      for(int i = 0; i<5; i++){
        Rwheel.write(MSTOPdeg+MSPEED, 150);
        Lwheel.write(MSTOPdeg+MSPEED, 150);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        delay(300);
      }
      Rwheel.write(MSTOPdeg);
      Lwheel.write(MSTOPdeg);
      delay(3000);
      command="";
    }else if(compareString(command, "sufia")){///////////////////////////////////////////////////////////////////////////
      Serial.println("sufia");
      command="";
    }else if(compareString(command, "red")){///////////////////////////////////////////////////////////////////////////
      Serial.println("red");
      command="";
    }else if(compareString(command, "green")){///////////////////////////////////////////////////////////////////////////
      Serial.println("green");
      command="";
    }else if(compareString(command, "blue")){///////////////////////////////////////////////////////////////////////////
      Serial.println("blue");
      command="";
    }else if(compareString(command, "hymn")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      mission = true;
      Serial.println("hymn");
      delay(2000);
      Serial.println("red");
      for (int pos = 544; pos <= 2000; pos += 3) { 
        rhand.write(pos);            
        delay(2);                       
      }
      rhand.write(2100);
      delay(8000);   
      for (int pos = 1200; pos <= 1600; pos += 1) {
        head.write(pos);
        delay(2);                      
      }
      delay(3000); 
      for (int pos = 1600; pos >= 1200; pos -= 1) { // in steps of 1 degree
        head.write(pos); 
        delay(2);                       
      }
        
      delay(6000);
      for (int pos = 2400; pos >= 1000; pos -= 1) { 
        lhand.write(pos);
        delay(1);                       
      }
      for (int pos = 2000; pos <= 2200; pos += 3) { 
        rhand.write(pos);           
        delay(2);                     
      }
      delay(1000);   
      for (int pos = 1200; pos <= 1600; pos += 1) {
        head.write(pos);
        delay(2);                       
      }
      delay(6000); 
      for (int pos = 1600; pos >= 1200; pos -= 1) {
        head.write(pos); 
        delay(2);                       
      } 
      delay(3000); 
      for (int pos = 1000; pos <= 2000; pos += 2) {
        lhand.write(pos);
        delay(3);                       
      }
      for (int pos = 1200; pos >= 700; pos -= 2) { 
        waist.write(pos); 
        delay(3);                       
      } 
      for (int pos = 1250; pos <= 2100; pos += 2) {
        rarm.write(pos);
        delay(3);                       
      }
      delay(2000);
      for (int pos = 2100; pos >= 544; pos -= 2) { 
        if(pos>1250)rarm.write(pos); 
        rhand.write(pos);
        delay(3);                       
      } 
      for (int pos = 700; pos <= 1700; pos += 2) { 
        waist.write(pos); 
        delay(3);                       
      } 
      for (int pos = 2000; pos >= 1400; pos -= 2) { 
        lhand.write(pos);
        delay(3);                       
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      delay(2000);
      gas.write(1300);
      // for (int pos = 1400; pos >= 2400; pos += 2) { 
      //   lhand.write(pos);
      //   delay(3);                       
      // }
      for (int pos = 1700; pos >= 1200; pos -= 2) { 
        waist.write(pos);
        delay(3);                      
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      delay(2000);
      gas.write(1300);
      delay(1000);
      command="";
    }else if(compareString(command, "wreck")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("ashti");
        head.write(1800);
        Serial.println("red");
        delay(1500);
        Serial.println("rgb-off");
        head.write(800);
        Serial.println("green");
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg+MSPEED,150);
        rhand.write(1000);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg+MSPEED,150);
        rhand.write(1000);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg+MSPEED,150);
        rhand.write(1000);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        head.write(1200);
        delay(300);
        Rwheel.write(MSTOP);
        Lwheel.write(MSTOP);
        delay(3000);
        head.write(1200);
        for (int pos = 800; pos <= 1600; pos += 3) { 
    rhand.write(pos);          
    delay(1);                     
  }
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg+MSPEED,150);
        rhand.write(1400);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(1600);
        delay(300);
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg+MSPEED,150);
        rhand.write(1400);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(1600);
        delay(300);
        Rwheel.write(MSTOP);
        Lwheel.write(MSTOP);
      command="";
    }else if(compareString(command, "optimus")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
        Serial.println("optimus");
        Serial.println("green");
        Rwheel.write(MSTOPdeg-MSPEED, 150);
        Lwheel.write(MSTOPdeg+MSPEED, 150);
        rhand.write(1000);
        delay(300);
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
        Rwheel.write(MSTOPdeg-MSPEED, 150);
        Lwheel.write(MSTOPdeg+MSPEED, 150);
        rhand.write(1000);
        delay(300);
        Rwheel.write(MSTOPdeg+MSPEED,150);
        Lwheel.write(MSTOPdeg-MSPEED,150);
        rhand.write(800);
        delay(300);
        Rwheel.write(MSTOP);
        Lwheel.write(MSTOP);
      
      command="";
    }else if(compareString(command, "ancient")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("green");
      Serial.println("king");
      for (int pos = 544; pos <= 2200; pos += 3) {
        rhand.write(pos);         
        delay(2);                     
      }
      head.write(1700);
      delay(5000);
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
      rhand.write(1000);
      head.write(1200);
      Serial.println("blue");
      delay(5000);
      Rwheel.write(MSTOPdeg+MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
      rhand.write(1400);
      head.write(1600);
      delay(5000);
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg-MSPEED, 150);
      rhand.write(2200);
      Serial.println("blue");
      delay(15000);
      head.write(1200);
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
      rhand.write(1000);
      delay(5000);
      Serial.println("green");
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      command="";
    }else if(compareString(command, "dance1")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("azari");
      Rwheel.write(MSTOPdeg+MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
      for (int pos = rhand.read(); pos <= 1600; pos += 3) { 
        rhand.write(pos);            
        delay(2);                   
      }
      for (int pos = lhand.read(); pos >= 700; pos -= 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      for(int i = 0; i<15; i++){
        for (int pos = MIN_RARM_ANGLE; pos <= MAX_RARM_ANGLE; pos += 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
        for (int pos = MAX_RARM_ANGLE; pos >= MIN_RARM_ANGLE; pos -= 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
      }
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg-MSPEED, 150);
      for (int pos = 700; pos <= 1200; pos += 2) { 
       lhand.write(pos);            
        delay(2);                   
      }
      for (int pos = 1600; pos <= 2400; pos += 2) { 
        rhand.write(pos);            
        delay(2);                   
      }
      for(int i = 0; i<15; i++){
        for (int pos = MIN_RARM_ANGLE; pos <= MAX_RARM_ANGLE; pos += 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
        for (int pos = MAX_RARM_ANGLE; pos >= MIN_RARM_ANGLE; pos -= 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
      } 
      Rwheel.write(MSTOPdeg+MSPEED, 150);
      Lwheel.write(MSTOPdeg+MSPEED, 150);
      for (int pos = 700; pos <= 2200; pos += 3) { 
        rhand.write(pos);            
        delay(2);                   
      }
      for (int pos = 1200; pos >= 1600; pos += 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      for(int i = 0; i<15; i++){
        for (int pos = MIN_RARM_ANGLE; pos <= MAX_RARM_ANGLE; pos += 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
        for (int pos = MAX_RARM_ANGLE; pos >= MIN_RARM_ANGLE; pos -= 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
      }
      Rwheel.write(MSTOPdeg-MSPEED, 150);
      Lwheel.write(MSTOPdeg-MSPEED, 150);
      for (int pos = 1600; pos >= 1200; pos -= 2) { 
       lhand.write(pos);            
        delay(2);                   
      }
      for (int pos = 2200; pos >= 2400; pos -= 2) { 
        rhand.write(pos);            
        delay(2);                   
      }
      for(int i = 0; i<15; i++){
        for (int pos = MIN_RARM_ANGLE; pos <= MAX_RARM_ANGLE; pos += 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
        for (int pos = MAX_RARM_ANGLE; pos >= MIN_RARM_ANGLE; pos -= 2) {
          rarm.write(pos); 
          larm.write(pos);        
          delay(1);                      
        }
      }
      delay(1000);
      
      command="";
    }else if(compareString(command, "voicemode")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      if(VoiceMode == 0){
        VoiceMode++;
        EEPROM.write(0, VoiceMode);
        Serial.println("success");
      }else if(VoiceMode == 1){
        VoiceMode=0;
        EEPROM.write(0, VoiceMode);
      }else{
        VoiceMode=0;
        EEPROM.write(0, VoiceMode);
      }
      
      command="";
    }else if(compareString(command, "dance2")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("setar");
      Rwheel.write(MSTOPdeg+MSPEED, 10);
      Lwheel.write(MSTOPdeg+MSPEED, 10);
      for (int pos = rhand.read(); pos <= 2400; pos += 1) { 
        rhand.write(pos);            
        delay(10);                   
      }
      for (int pos = lhand.read(); pos >= 600; pos -= 1) { 
        lhand.write(pos);            
        delay(10);                   
      }
      for (int pos = rarm.read(); pos <= 2200; pos += 1) { 
        rarm.write(pos);            
        delay(10);                   
      }
      delay(55000);
      command="";
    }else if(compareString(command, "osho")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
        Serial.println("osho");
      command="";
    }else if(compareString(command, "hafez")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("hafez");
      delay(4000);
      for (int pos = lhand.read(); pos >= 600; pos -= 1) { 
        lhand.write(pos);            
        delay(2);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      delay(2000);
      gas.write(1300);
      delay(4000);
      for (int pos = 600; pos <= 2200; pos += 1) { 
        lhand.write(pos);            
        delay(2);                   
      }
      delay(2000);
      Rwheel.write(MSTOPdeg+MSPEED, 10);
      Lwheel.write(MSTOPdeg+MSPEED, 10);
      for (int pos = rhand.read(); pos <= 2400; pos += 1) { 
        rhand.write(pos);            
        delay(8);                   
      }
      delay(4000);
      Rwheel.write(MSTOPdeg);
      Lwheel.write(MSTOPdeg);
      for (int pos = rarm.read(); pos <= 2200; pos += 1) { 
        rarm.write(pos);            
        delay(6);                   
      }

      delay(2000);
      for (int pos = 1200; pos <= 1600; pos += 1) {
        head.write(pos);
        delay(6);                      
      }
      delay(3000); 
      for (int pos = 1600; pos >= 1200; pos -= 1) { // in steps of 1 degree
        head.write(pos); 
        delay(6);                       
      }
      delay(4000);
      for (int pos = 2200; pos >= 800; pos -= 1) { 
        rarm.write(pos);            
        delay(6);                   
      }
      delay(4000);
      for (int pos = 2200; pos >= 1800; pos -= 1) { 
        lhand.write(pos);            
        delay(2);                   
      }
      for (int pos = 1200; pos <= 1600; pos += 1) {
        waist.write(pos);       
        delay(5);                      
      }
      delay(3000);
      for (int pos = 1600; pos >= 800; pos -= 1) {
        waist.write(pos);       
        delay(5);                      
      }
      for (int pos = 2400; pos >= 600; pos -= 1) { 
        rhand.write(pos);            
        delay(6);                   
      }
      delay(500);
      for (int pos = 800; pos <= 1200; pos += 1) {
        waist.write(pos);       
        delay(6);                      
      }
      command="";
    }else if(compareString(command, "scene1")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      Serial.println("class");
      for (int pos = 544; pos <= 2200; pos += 3) {
        rhand.write(pos);          
        delay(2);                      
      }delay(1000);
      for (int pos = 2200; pos >= 544; pos -= 3) {
        rhand.write(pos);          
        delay(2);                      
      }
      Rwheel.write(MSTOPdeg+MSPEED,MACCEL);//rr
      Lwheel.write(MSTOPdeg+MSPEED,MACCEL);
      delay(3000);
      Rwheel.write(MSTOPdeg-MSPEED, MACCEL);//f
      Lwheel.write(MSTOPdeg+MSPEED, MACCEL);
      delay(4000);
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      for (int pos = 1200; pos <= 1700; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(2000);
      for (int pos = 1700; pos >= 1100; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      Rwheel.write(MSTOPdeg-MSPEED, MACCEL);//f
      Lwheel.write(MSTOPdeg+MSPEED, MACCEL);
      delay(4000);
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      for (int pos = 1200; pos <= 1700; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(2000);
      for (int pos = 1700; pos >= 1100; pos -= 1) {
        waist.write(pos);       
        delay(2);                      
      }
      Rwheel.write(MSTOPdeg-MSPEED, MACCEL);//f
      Lwheel.write(MSTOPdeg+MSPEED, MACCEL);
      delay(4000);
      Rwheel.write(MSTOP);
      Lwheel.write(MSTOP);
      for (int pos = 1200; pos <= 1700; pos += 1) {
        waist.write(pos);       
        delay(2);                      
      }
      delay(3000);

      command="";
    }else if(compareString(command, "scene2")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;

      command="";
    }else if(compareString(command, "figure1")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      if(figurcounter1==0){///////////////////
      head.write(1300);   
        for (int pos = rhand.read(); pos <= 1600; pos += 3) { 
        rhand.write(pos);            
        delay(2);                   
      }
      for (int pos = lhand.read(); pos >= 1300; pos -= 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      rarm.write(2200);
      for (int pos = waist.read(); pos >= MIN_WAIST_ANGLE; pos -= 1) { 
        waist.write(pos);            
        delay(5);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      for (int pos = waist.read(); pos <= MAX_WAIST_ANGLE; pos += 1) { 
        waist.write(pos);            
        delay(5);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      head.write(1600); 
      for (int pos = lhand.read(); pos >= 1000; pos -= 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      for (int pos = waist.read(); pos >= MIN_WAIST_ANGLE; pos -= 1) { 
        waist.write(pos);            
        delay(5);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      for (int pos = waist.read(); pos <= MAX_WAIST_ANGLE; pos += 1) { 
        waist.write(pos);            
        delay(5);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      head.write(1800); 
      for (int pos = lhand.read(); pos >= 800; pos -= 3) { 
        lhand.write(pos);            
        delay(2);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      for (int pos = waist.read(); pos >= 1200; pos -= 1) { 
        waist.write(pos);            
        delay(5);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(500);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      figurcounter1++;
      }else if(figurcounter1==1){///////////////////
        for (int pos = 0; pos <= 1100; pos += 1) { 
        rhand.write(544+pos); 
        delay(2);
        lhand.write(2400-pos); 
        delay(2);                   
      }
      for (int pos = rarm.read(); pos <= 2200; pos += 3) { 
        rarm.write(pos);            
        delay(2);                   
      }
      for (int pos = 2200; pos >= 1250; pos -= 3) { 
        rarm.write(pos);            
        delay(2);                   
      }
      for (int pos = 1100; pos <= 1600; pos += 1) { 
        rhand.write(544+pos); 
        delay(2);
        lhand.write(2400-pos); 
        delay(2);                   
      }
      for (int pos = rarm.read(); pos <= 2200; pos += 3) { 
        rarm.write(pos);            
        delay(2);                   
      }
      for (int pos = 2200; pos >= 1250; pos -= 3) { 
        rarm.write(pos);            
        delay(2);                   
      }
      for (int pos = 1100; pos <= 2200; pos += 1) { 
        rhand.write(544+pos); 
        delay(2);
        lhand.write(2400-pos); 
        delay(2);                   
      }
      figurcounter1++;
      }else if(figurcounter1==2){////////////////
        for (int pos = waist.read(); pos >= MIN_WAIST_ANGLE; pos -= 1) { 
        waist.write(pos);            
        delay(5);                   
      }
      for (int pos = lhand.read(); pos >= 1300; pos -= 1) { 
        lhand.write(pos);            
        delay(5);                   
      }
      digitalWrite(spark, LOW);
      gas.write(700);
      delay(4000);
      digitalWrite(spark, HIGH);
      gas.write(1300);
      figurcounter1++;
      }else{
        figurcounter1=0;
      }
        
      command="";
    }else if(compareString(command, "figure2")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
        
      command="";
    }else if(compareString(command, "stopseq")){///////////////////////////////////////////////////////////////////////////
      Serial.println("stopaudio");
      Rwheel.write(MSTOPdeg);
      Lwheel.write(MSTOPdeg);
      command="";
    }else if(compareString(command, "reset")){///////////////////////////////////////////////////////////////////////////
      
      Serial.println("rgb-off");
      rarm.write(1250);
      delay(500);
      larm.write(2400);
      delay(500);
      rhand.write(MIN_RARM_ANGLE);
      delay(500);
      lhand.write(2400);
      delay(500);
      head.write(1200);
      delay(500);
      gas.write(1300);
      delay(500);
      waist.write(1200);
      delay(500);
      SeqCounter = 0;
      Serial.println("success");
      delay(1500);
      Serial.println("stopaudio");
      Serial.println("rgb-off");
      command="";
    }else if(compareString(command, "random")){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      
      
      int randNumber1 = random(rand_min,rand_max);
      if(randNumber1 == 0){
        Serial.println("ai");
      }else if(randNumber1 == 1){
        Serial.println("optimus");
      }else if(randNumber1 == 2){
        Serial.println("sufia");
      }else if(randNumber1 == 3){
        
      }else if(randNumber1 == 4){
        Serial.println("aha");
      }else if(randNumber1 == 5){
        Serial.println("king");
      }else if(randNumber1 == 6){
        Serial.println("cat");
      }else if(randNumber1 == 7){
        
      }

  command="";
    }else if(compareString(command, "task") && VoiceMode == 1){///////////////////////////////////////////////////////////////////////////
      autoHome = true;
      int randNumber1 = random(rand_min,6);
      if(randNumber1 == 0){
        Serial.println("ai");
      }else if(randNumber1 == 1){
        Serial.println("optimus");
      }else if(randNumber1 == 2){
        Serial.println("sufia");
      }else if(randNumber1 == 3){
        Serial.println("cat");
      }else if(randNumber1 == 4){
        Serial.println("king");
      }else if(randNumber1 == 5){
        
      }
      command="";
    }///////////////////////////////////////////////////////////////////////////
}
boolean compareString(String a, String b) {
  if (a.length() != b.length() + 1) {
    return false;
  }
  for (int i = 0; i < a.length() - 1; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}