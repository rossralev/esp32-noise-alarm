
#include <WiFi.h>
#include <WiFiClient.h>
#include "IFTTTWebhook.h"

#include "Settings.h"
#include "HttpApp.h"

#define EVENT_NAME "noisealarm"

const int adminModePin = 1;
int isAdmin = 0;

Settings *settings;
HttpApp *httpApp;

IFTTTWebhook* wh;
int sensorPin = A0;
int buttonPin = 1; 
int sensorValue = 0;
int st = 3;
unsigned long lastRead = 0;
unsigned long lastAlarm = 0;


void setUpWifiClient(String ssid, String pass) {
  
  // Connect to WiFi network

  int len =  ssid.length() + 0;
  char ssidChar[len];
  ssid.toCharArray(ssidChar, len);

  len =  pass.length() + 0;
  char passChar[len];
  pass.toCharArray(passChar, len);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidChar, passChar);
  Serial.println(ssidChar);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    Serial.println(WL_CONNECTED);
    Serial.println(ssidChar);
    Serial.println(passChar);
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

  pinMode(16, INPUT);
  delay(100);
  isAdmin  = digitalRead(16);
  settings = new Settings("/settings.txt");

  if( isAdmin == HIGH ) {
    Serial.println("Setup Client");
    setUpWifiClient(settings->getSSID(), settings->getSSIDPass());
  } else {
    Serial.println("Setup Access Point");
    setUpWifiAP("noisealarm","");
  }
  
  httpApp = new HttpApp(settings);
  
  int len =  settings->getIFTTTKey().length() + 1;
  char key[len];
  settings->getIFTTTKey().toCharArray(key, len);
  Serial.println(key);

  wh = new IFTTTWebhook(key, EVENT_NAME);

  Serial.println("HTTP server started");
}

void loop(void) {

  httpApp->handle();

  if(millis() - lastRead > 10000){

    int newValue = analogRead(sensorPin);
    if( abs(newValue-sensorValue) > st && newValue >= settings->getTreshold() ) {
      Serial.println(sensorValue);
      sensorValue = newValue;
      if(millis()-lastAlarm > 10000){

        char charsValue[16];
        itoa(sensorValue, charsValue, 10);

        int len =  settings->getIFTTTKey().length() + 1;
        char dn[len];
        settings->getDeviceName().toCharArray(dn, len);
        Serial.println(dn);
        wh->trigger(dn, charsValue);

        lastAlarm = millis();
      }
    }

    lastRead=millis();

  }

}
