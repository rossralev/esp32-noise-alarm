
#include <WiFi.h>
#include <WiFiClient.h>
#include "AnotherIFTTTWebhook.h"
#include "EEPROM.h"

#include "Settings.h"
#include "HttpApp.h"

#define EVENT_NAME "noisealarm"

const int adminModePin = 16;
int isAdmin = 0;

int admSettingsAddr = 0;

Settings *settings;
HttpApp *httpApp;

int sensorPin = 34;
int buttonPressed = 0; 
int sensorValue = 0;
int st = 3;
unsigned long lastRead = 0;
unsigned long lastAlarm = 0;


void setUpWifiClient(String ssid, String pass) {
  
  // Connect to WiFi network

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println(ssid);
  Serial.println(ssid.length());

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    Serial.println(WL_CONNECTED);
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setUpWifiAP(char* ssid, char* pass) {
  
  delay(1000);
  Serial.println();
  Serial.print("Configuring access point...");

  if(!WiFi.enableAP(true)) {
      // enable AP failed
      Serial.println("[AP] enableAP failed!\n");
      return;
  }
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, pass);
  Serial.println(WiFi.softAPIP());
  
}

void setup(void) {

  
  Serial.begin(115200);
  delay(500);
  Serial.println("Will Start");
  delay(500);

  if (EEPROM.begin(64))
  {
    
    if(EEPROM.read(admSettingsAddr) == 255){
      isAdmin = 1;
    }
    Serial.println("EEPROM");
    Serial.println(EEPROM.read(admSettingsAddr));
    Serial.println(isAdmin);
    delay(5000);
    EEPROM.write(admSettingsAddr, 0);
    Serial.println(EEPROM.read(admSettingsAddr));
     EEPROM.commit();
  }

  pinMode(17, INPUT);
  pinMode(16, INPUT);

  settings = new Settings("/settings.txt");

  if( isAdmin == 0 ) {
    Serial.println("Setup Client");
    setUpWifiClient(settings->getSSID(), settings->getSSIDPass());
  } else {
    Serial.println("Setup Access Point");
    setUpWifiAP("noisealarm","");
  }
  
  httpApp = new HttpApp(settings);
  
  int len =  settings->getIFTTTKey().length();
  char key[len];
  settings->getIFTTTKey().toCharArray(key, len);

  Serial.println("HTTP server started");
}

void loop(void) {

  httpApp->handle();

  if(millis() - lastRead > 1000){

    int newValue = analogRead(sensorPin);
    int a = analogRead(sensorPin);
    newValue = max(newValue, a);
    a = analogRead(sensorPin);
    newValue = max(newValue, a);
    delay(200);

    Serial.println(digitalRead(17));
    Serial.println(digitalRead(16));
  
    // Enter setup after restart
    if(digitalRead(adminModePin) == HIGH) {
      if(buttonPressed++ > 10) {
        EEPROM.write(admSettingsAddr, 255);
        Serial.println("Set Admin on next start...");
        delay(100);
        EEPROM.commit();
        ESP.restart();
      }
    } else {
      buttonPressed = 0;
    }

    httpApp->state = newValue;
    Serial.println(newValue);
    
    if( abs(newValue-sensorValue) > st && (newValue >= settings->getTreshold() ||  newValue <= settings->getTresholdMin())) {
     
      sensorValue = newValue;

      if(millis()-lastAlarm > 10000){

        send_webhook(EVENT_NAME, settings->getIFTTTKey(), settings->getDeviceName(), String(sensorValue), "");

        lastAlarm = millis();
        delay(100);
      }
    }

    lastRead=millis();

  }

}
