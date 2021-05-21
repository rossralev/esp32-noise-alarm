#include "Arduino.h"
#include "Settings.h"
#include <FS.h>
#include <SPIFFS.h>

Settings::Settings( char* path) {

  _path = path;

  Serial.println();

  if (!SPIFFS.begin(false)) {
    Serial.println("FFat mount failed, trying to format");
    delay(1000);
  }

  if(SPIFFS.exists(_path)) {
    load();
  }

};

void Settings::load() {
  Serial.printf("Reading file: %s\n", _path);
 
  File file = SPIFFS.open(_path, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");

  _appUser     = readLine(file);
  _appPass     = readLine(file);
  _ssid        = readLine(file);
  _ssidPass    = readLine(file);
  _iftttKey    = readLine(file);
  _treshold    = readLine(file);
  _tresholdMin = readLine(file);
  _deviceName  = readLine(file);

  file.close();
  Serial.println("End Read ");
}

String Settings::readLine(File f) {

  char x;
  String ret;

  while(f.available() && x != '\n') {
    x = f.read();
    if(x != '\n') {
      ret += x;
    }
  }

  return ret; 
}

void Settings::save() {
  
  Serial.printf("Writing file: %s\n", _path);

  File file = SPIFFS.open(_path, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  Serial.println("Will write");
  
  //Serial.println(this->_appUser);
  file.println(this->_appUser);

  //Serial.println(this->_appPass);
  file.println(this->_appPass);

  //Serial.println(this->_ssid);
  file.println(this->_ssid);

  //Serial.println(this->_ssidPass);
  file.println(this->_ssidPass);

  //Serial.println(this->_iftttKey);
  file.println(this->_iftttKey);

  //Serial.println(this->_treshold);
  file.println(this->_treshold);

  //Serial.println(this->_treshold);
  file.println(this->_tresholdMin);

  //Serial.println(this->_deviceName);
  file.println(this->_deviceName);

  file.close();

}

String Settings::getAppUser() {
    return _appUser;
}
String Settings::getAppPass() {
    return _appPass;
}
String Settings::getSSID() {
    return _ssid;
}
String Settings::getSSIDPass() {
    return _ssidPass;
}
String Settings::getIFTTTKey() {
    return _iftttKey;
}
int   Settings::getTreshold() {
    return _treshold.toInt();
}
int   Settings::getTresholdMin() {
    return _tresholdMin.toInt();
}
String Settings::getDeviceName() {
    return _deviceName;
}

void Settings::setAppUser(String val) {
    _appUser = val;
}
void Settings::setAppPass(String val) {
    _appPass = val;
}
void Settings::setSSID(String val) {
    _ssid = val;
}
void Settings::setSSIDPass(String val) {
    _ssidPass = val;
}
void Settings::setIFTTTKey(String val) {
    _iftttKey = val;
}
void Settings::setTreshold(int val) {
    _treshold = String(val);
}
void Settings::setTresholdMin(int val) {
    _tresholdMin = String(val);
}
void Settings::setDeviceName(String val) {
    _deviceName = val;
}