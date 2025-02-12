/*
    AIBOT Ai Board Sketch
    Version 1.o
    Written by Sorush Moradisani
*/
#include <Arduino.h>
#include <Audio.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <Preferences.h>

const char* ssid = "your ssid";
const char* password = "your password";

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26
#define SetupMode     13
Audio audio;

// IPAddress IP(192, 168, 70, 71);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 0, 0);
// IPAddress primaryDNS(8, 8, 8, 8); //optional
// IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebSocketsServer webSocket = WebSocketsServer(9999);

String command,arg;;
bool commandEnd=false;
char inByte;
bool ans = false;


Preferences preferences;
String SavedSSID;
String SavedPassword;

bool isClientConnected;
String cmd, Text;


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            {
              webSocket.sendTXT(num, "Disconnected!");
              isClientConnected = false;
            }
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.print("Connected IP address:");
                Serial.println(ip);
                isClientConnected = true;
            }
            break;
    case WStype_TEXT:
            {
              String msg = (char*)payload;
              Text = msg;
              Serial.println(msg);
              audio.connecttospeech(msg.c_str(), "en");
              if(msg.startsWith("ssid=")){
                preferences.begin("credentials", false);
                preferences.putString("ssid", msg.substring(5)); 

                Serial.println("Username Saved");

                preferences.end();

              }else if(msg.startsWith("password=")){
                preferences.begin("credentials", false);
                preferences.putString("password", msg.substring(9));

                Serial.println("Password Saved");

                preferences.end();

              }else if(msg.startsWith("getSSID")){


              }else if(msg.startsWith("getPassword")){


              }else if(msg.startsWith("reset")){
                //digitalWrite(LED_BUILTIN, HIGH);
                //delay(3000);
                //digitalWrite(LED_BUILTIN, LOW);
              }
              
            }
    case WStype_BIN:
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
    }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();
  pinMode(SetupMode, INPUT_PULLUP);
  preferences.begin("credentials", false);
  SavedSSID = preferences.getString("ssid", ""); 
  SavedPassword = preferences.getString("password", "");
  // if(!WiFi.config(IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  // }
if(digitalRead(SetupMode) == 1){
  if (SavedSSID == "" ){
    Serial.println("No values saved for ssid or password, Using default Credentials...");
    WiFi.begin(ssid, password);
    Serial.print("Connecting to wifi..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP()); 
  }else{
    // Connect to Wi-Fi   ssid=OMEN   password=Sorush3196
    WiFi.begin(SavedSSID.c_str(), SavedPassword.c_str());
    Serial.print("Connecting to ");
    Serial.print(SavedSSID);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP());  
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100);
  audio.connecttospeech("AI Connected!", "en");
}
}


void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  audio.loop();
  if (Serial.available() > 0) {

      if(commandEnd){
      command="";
      arg="";
      commandEnd=false;
    }
      inByte = Serial.read();
      if (inByte != '\n') {
        if (inByte == '='&& !ans) {
          ans = true;     
        } else if(!ans){
          command += inByte;
        }else if (ans){
          arg += inByte;
        }
      }else{
        ans = false;
        commandEnd=true;
        Serial.print(command);
        Serial.print("=");
        Serial.println(arg);
        if(command == "ssid"){
          preferences.begin("credentials", false);
          preferences.putString("ssid", arg); 
          Serial.print("Username Saved: ");
          Serial.println(preferences.getString("ssid", ""));
          preferences.end();
        }else if(command=="password"){
          preferences.begin("credentials", false);
          preferences.putString("password", arg); 
          Serial.print("Password Saved: ");
          Serial.println(preferences.getString("password", ""));
          preferences.end();
        }
      }
  }
}
void audio_info(const char *info) {
  Serial.print("audio_info: "); Serial.println(info);
}
