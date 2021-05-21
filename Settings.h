#ifndef Settings_h
#define Settings_h

#include <FS.h>
#include <FFat.h>
#include "Arduino.h"

class Settings {
  public:
    Settings(char* path);
    void load();
    void save();
    String getAppUser();
    String getAppPass();
    String getSSID();
    String getSSIDPass();
    String getIFTTTKey();
    int   getTreshold();
    int   getTresholdMin();
    String getDeviceName();
    void  setAppUser(String val);
    void  setAppPass(String val);
    void  setSSID(String val);
    void  setSSIDPass(String val);
    void  setIFTTTKey(String val);
    void  setTreshold(int val);
    void  setTresholdMin(int val);
    void  setDeviceName(String val);
  private:
    String readLine(File f);
    String _appUser="admin";
    String _appPass="noisealarm";
    String _ssid="ssid";
    String _ssidPass="ssidpass";
    String _iftttKey="IFTTT KEY";
    String _treshold="85";
    String _tresholdMin="75";
    String _deviceName="Noise Alarm Name";
    char* _path;

};

#endif
