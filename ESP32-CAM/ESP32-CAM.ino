/*
    AIBOT ESP32-CAM Sketch
    Version 1.1
    Written by Sorush Moradisani
*/
#include "esp_camera.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "fr_flash.h"
#include "bridge.h"
#include <Preferences.h>

#define Reset 12
#define RED 15 // pin 12 can also be used
#define GREEN 14
#define BLUE 2
#define LED_BUILTIN 4
bool INT_LED = false;
bool RED_LED = false;
bool GREEN_LED = false;
bool BLUE_LED = false;
// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"
// Define an SSID name and a password to access the ESP32-CAM access point.


const char* ssid = "your ssid";
const char* password = "your password";

// IPAddress IP(192, 168, 70, 70);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 0, 0);
// IPAddress primaryDNS(8, 8, 8, 8); //optional
// IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebSocketsServer webSocket = WebSocketsServer(9898);
String command, arg;
char inByte;
bool ans = false;
bool commandEnd=0;
bool isClientConnected;
String GlobalIP;

Preferences preferences;
String SavedSSID;
String SavedPassword;

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
                webSocket.sendTXT(num, "ip=" + GlobalIP);
                isClientConnected = true;
            }
            break;
    case WStype_TEXT:
            {
              String msg = (char*)payload;
              Serial.println(msg);

              if(msg.startsWith("ssid=")){
                preferences.begin("credentials", false);
                preferences.putString("ssid", msg.substring(5)); 

                Serial.println("Username Saved");

                preferences.end();

              }else if(msg.startsWith("password=")){
                preferences.begin("credentials", false);
                preferences.putString("password", msg.substring(9));

                Serial.println("Username Saved");

                preferences.end();

              }else if(msg.startsWith("getSSID")){


              }else if(msg.startsWith("getPassword")){


              }else if(msg.startsWith("light")){
                if(INT_LED){
                  digitalWrite(LED_BUILTIN, LOW);
                  INT_LED = false;
                }else{
                  digitalWrite(LED_BUILTIN, HIGH);
                  INT_LED = true;
                }
                Serial.println(SavedSSID);
                Serial.println(SavedPassword);
              }else if(msg.startsWith("red")){
                if(RED_LED){
                  //digitalWrite(RED, LOW);
                  RED_LED = false;
                }else{
                  //digitalWrite(RED, HIGH);
                  RED_LED = true;
                }
              }else if(msg.startsWith("green")){
                if(GREEN_LED){
                  //digitalWrite(GREEN, LOW);
                  GREEN_LED = false;
                }else{
                  //digitalWrite(GREEN, HIGH);
                  GREEN_LED = true;
                }
              }else if(msg.startsWith("blue")){
                if(BLUE_LED){
                  //digitalWrite(BLUE, LOW);
                  BLUE_LED = false;
                }else{
                  //digitalWrite(BLUE, HIGH);
                  BLUE_LED = true;
                }
              }else if(msg.startsWith("capture")){
                is_enrolling = 1;
                webSocket.broadcastTXT("CAPTURING");
              }else if(msg.startsWith("recognise")){
                detection_enabled = 1;
                recognition_enabled = 1;
                webSocket.broadcastTXT("RECOGNISING");
              }else if(msg.startsWith("deleteall")){

              }else if(msg.startsWith("detect")){
                detection_enabled = 1;
                recognition_enabled = 0;
                // Serial.print("detection_enabled=");
                // Serial.println(detection_enabled);
              }else if(msg.startsWith("stream")){
                detection_enabled = 0;
                recognition_enabled = 0;
                is_enrolling = 0;
                webSocket.broadcastTXT("STREAMING");
              }else if(msg.startsWith("reset")){
                digitalWrite(LED_BUILTIN, LOW);
                digitalWrite(RED, LOW);
                digitalWrite(GREEN, LOW);
                digitalWrite(BLUE, LOW);
                INT_LED = false;
                RED_LED = false;
                GREEN_LED = false;
                BLUE_LED = false;
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

void startCameraServer();

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode (LED_BUILTIN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(33, OUTPUT); 
  pinMode(Reset, INPUT_PULLUP);
  digitalWrite(33, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_CIF);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  preferences.begin("credentials", false);
  if(digitalRead(Reset) == LOW){
    preferences.putString("ssid", ""); 
    preferences.putString("password", "");
  }
  // Serial.print("ResetPin ");
  // Serial.println(digitalRead(Reset));
  SavedSSID = preferences.getString("ssid", ""); 
  SavedPassword = preferences.getString("password", "");
  preferences.end();
  // if(!WiFi.config(IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  // }
  if (SavedSSID == "" || SavedPassword == ""){
    Serial.println("No values saved for ssid or password, Using default values...");
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP()); 
  }else{
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(SavedSSID.c_str(), SavedPassword.c_str());
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP());  
  }

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  startCameraServer();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.print("Camera Ready! Use ");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
  GlobalIP = WiFi.localIP().toString();
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();

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
