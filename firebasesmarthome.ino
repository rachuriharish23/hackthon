#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define WIFI_SSID "RPI"
#define WIFI_PASSWORD "9876543210"
#define API_KEY "AIzaSyDJ9umSLW8h5D0hMv5TaqXnqNHSffpElbc"
#define DATABASE_URL "https://test123-2960b-default-rtdb.firebaseio.com/"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int quality = 0;
String intValue;

void setup(){
  Serial.begin(115200);
  pinMode(D0,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
 int Temp = random(1,10);
 int Humid= random(1,100);
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.setInt(&fbdo, "database/temp", Temp)){
      Serial.println("value: " + Temp);
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("Failed REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.RTDB.setInt(&fbdo, "AITS/humid", Humid)){
      Serial.println("value: " + Temp);
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("Failed REASON: " + fbdo.errorReason());
    }
    delay(1000);
    if (Firebase.RTDB.getString(&fbdo, "/home/lights"))
    {
      intValue = fbdo.stringData();
      String mySubString = intValue.substring(2, 4);
      Serial.println(intValue);
      Serial.println(mySubString);
      if (mySubString == "on")
      {
        digitalWrite(D0, LOW); // turn the LED on (HIGH is the voltage level)
        Serial.println("LED ON");
        delay(100); 
      }
      else if (mySubString == "of")
      {
        digitalWrite(D0, HIGH); // turn the LED on (HIGH is the voltage level)
        Serial.println("LED OFF");
        delay(100);  
      }  
    delay(100);
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    delay(100);
  }
}
