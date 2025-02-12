/*
    AIBOT Secondary Arduino Sketch
    Version 1.0
    Written by Sorush Moradisani
*/
// TMRpcm audio;
// audio.play("filename");    plays a file
// audio.play("filename",30); plays a file starting at 30 seconds into the track
// audio.play(F("filename")); plays a file storing the string in program memory instead of RAM
// audio.speakerPin = 11;     set to 5,6,11 or 46 for Mega, 9 for Uno, Nano, etc.
// audio.disable();           disables the timer on output pin and stops the music
// audio.stopPlayback();      stops the music, but leaves the timer running
// audio.isPlaying();         returns 1 if music playing, 0 if not
// audio.pause();             pauses/unpauses playback
// audio.quality(1);          Set 1 for 2x oversampling
// audio.volume(0);           1(up) or 0(down) to control volume
// audio.setVolume(0);        0 to 7. Set volume level
// audio.loop(1);             0 or 1. Can be changed during playback for full control of looping. 
// Main formats: WAV files, 8-bit, 8-32khz Sample Rate, mono. See the wiki for other options. 

#include<SD.h>                //declare SD library for SD cards
#define SD_ChipSelectPin 10   //select sd chip select pin 10
#include <TMRpcm.h>            //library to play audio from sd card by arduino
#include"SPI.h"
#include"Timer.h"

Timer t;  

TMRpcm tmr;              //name tmrpcm
#define RED A2 
#define GREEN A1
#define BLUE A0
bool RED_LED = false;
bool GREEN_LED = false;
bool BLUE_LED = false;

String command;
bool commandEnd=false;
char inByte;
int arg = 0;
int heycount = 0;
int oshocount = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  tmr.speakerPin=9;   //Speaker pin 9,audio out pin is 9
  tmr.setVolume(4);
   if(!SD.begin(SD_ChipSelectPin))   //this runs until SD is connected
  {
    Serial.println("SD FAIL");
    return; 
  }else{
    Serial.println("SD OK");
  }
  t.every(1000, StatusReport);
}

void loop() {
  t.update();
  // put your main code here, to run repeatedly:
  if (Serial.available()>0) {

      if(commandEnd){
      command="";
      commandEnd=false;
    }
      inByte = Serial.read();
      if (inByte != '\n') {
        if (inByte == '=') {
          arg = Serial.parseInt();
          //Serial.println(arg);      
        } else {
          command += inByte;
        }
      }else{
        commandEnd=true;
        Serial.print(command);
        Serial.print("=");
        Serial.println(arg);
        //if(!tmr.isPlaying())
        execute(command, arg);
        if(compareString(command, "stopseq") || compareString(command, "stopaudio")){
          tmr.stopPlayback();
        }
      }
  }
}
void execute(String command, int arg){
  if(compareString(command, "setar")){
    tmr.play("setar.wav");
  }else if(compareString(command, "aha")){
    tmr.play("aha.wav");
  }else if(compareString(command, "ai")){
    tmr.play("ai.wav");
  }else if(compareString(command, "aibot")){
    tmr.play("aibot.wav");
  }else if(compareString(command, "aibot2")){
    tmr.play("aibot2.wav");
  }else if(compareString(command, "ashti")){
    tmr.play("ashti.wav");
  }else if(compareString(command, "azari")){
    tmr.play("azari.wav");
  }else if(compareString(command, "buiru")){
    tmr.play("buiru.wav");
  }else if(compareString(command, "class")){
    tmr.play("class.wav");
  }else if(compareString(command, "gol")){
    tmr.play("gol.wav");
  }else if(compareString(command, "hi")){
    tmr.play("hi.wav");
  }else if(compareString(command, "hey")){
    if(heycount==0){
      tmr.play("bale.wav");
      heycount++;
    }else if(heycount==1){
      tmr.play("janam.wav");
      heycount++;
    }else if(heycount==2){
      tmr.play("jan.wav");
      heycount++;
    }else{
      tmr.play("chi.wav");
      heycount=0;
    }
    
  }else if(compareString(command, "osho")){
    if(oshocount==0){
      tmr.play("osho1.wav");
      oshocount++;
    }else if(oshocount==1){
      tmr.play("osho2.wav");
      oshocount++;
    }else{
      tmr.play("osho3.wav");
      oshocount=0;
    }
    
  }else if(compareString(command, "hafez")){
    tmr.play("hafez.wav");
  }else if(compareString(command, "hidad")){
    tmr.play("hidad.wav");
  }else if(compareString(command, "hymn")){
    tmr.play("hymn.wav");
  }else if(compareString(command, "king")){
    tmr.play("king.wav");
  }else if(compareString(command, "kojai")){
    tmr.play("kojai.wav");
  }else if(compareString(command, "log")){
    tmr.play("log.wav");
  }else if(compareString(command, "no")){
    tmr.play("no.wav");
  }else if(compareString(command, "optimus")){
    tmr.play("optimus.wav");
  }else if(compareString(command, "past")){
    tmr.play("past.wav");
  }else if(compareString(command, "rap")){
    tmr.play("rap.wav");
  }else if(compareString(command, "setar")){
    tmr.play("setar.wav");
  }else if(compareString(command, "success")){
    tmr.play("success.wav");
  }else if(compareString(command, "sufia")){
    tmr.play("sufia.wav");
  }else if(compareString(command, "liftme")){
    tmr.play("liftme.wav");
  }else if(compareString(command, "red")){
    if(RED_LED){
      digitalWrite(RED, LOW);
      RED_LED = false;
    }else{
      digitalWrite(RED, HIGH);
      RED_LED = true;
    }
  }else if(compareString(command, "green")){
    if(GREEN_LED){
      digitalWrite(GREEN, LOW);
      GREEN_LED = false;
    }else{
      digitalWrite(GREEN, HIGH);
      GREEN_LED = true;
    }
  }else if(compareString(command, "blue")){
    if(BLUE_LED){
      digitalWrite(BLUE, LOW);
      BLUE_LED = false;
    }else{
      digitalWrite(BLUE, HIGH);
      BLUE_LED = true;
    }
  }else if(compareString(command, "rgb-off")){
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    RED_LED = false;
    GREEN_LED = false;
    BLUE_LED = false;
  }else{

  } 
command="";
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
void StatusReport(){
  Serial.print("audio=");
  Serial.println(tmr.isPlaying());
}