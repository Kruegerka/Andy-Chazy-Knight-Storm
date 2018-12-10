#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "sd-database-72aa2.firebaseio.com"
#define FIREBASE_AUTH "y0qs6nO5u5IEtAQR0eVdToE851SEFDU3DMqshf"

//Dorm Room Wifi
#define WIFI_SSID "SkyNet-2.4G"
#define WIFI_PASSWORD "spaceunderscore"

//School Wifi
//#define WIFI_SSID "MSOE-Guest"
//#define WIFI_PASSWORD ""

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.println("Establishing Connection");
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.println("ESP-12 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting To Data Base");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
  if(Firebase.success()){
    Serial.println("Data Base Connected");
  }else{
    Serial.println("Data base unreachable");
  }
  
}

void loop() {
  String firebaseResult = Firebase.getString("/BoardLED/status/");
  if(Firebase.success()){
    Serial.println(firebaseResult);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (firebaseResult.compareTo("ON")){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}