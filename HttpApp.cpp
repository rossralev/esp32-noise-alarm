#include "HttpApp.h"
#include <WebServer.h>
#include "Arduino.h"

HttpApp::HttpApp(Settings *settings) {
    _server = new WebServer(80);
    _settings = settings;
    _server->onNotFound( std::bind(&HttpApp::handle_http_not_found, this) );
    _server->on("/settings", std::bind(&HttpApp::handle_http_settings, this) );
    _server->begin();

}

void HttpApp::handle() {
     _server->handleClient();
}

void HttpApp::handle_http_not_found() {
    _server->send(404, "text/plain", "Not Found");
}

void HttpApp::handle_http_settings() {
    Serial.println("Enter Settings");

    _settings->getAppUser().trim();
    int len =  _settings->getAppUser().length() + 0;
    char usr[len];
    _settings->getAppUser().toCharArray(usr, len);

    _settings->getAppPass().trim();
    len =  _settings->getAppPass().length() + 0;
    char pass[len];
    _settings->getAppPass().toCharArray(pass, len);

    Serial.println(usr);
    Serial.println(pass);

    //if (!_server->authenticate(usr[0], pass[0])) {
    //    return _server->requestAuthentication();
    //}    

    if(_server->method() == HTTP_POST) {
  
        for (uint8_t i = 0; i < _server->args(); i++) {

            String value = _server->arg(i);

            if(_server->argName(i) == "authuser"){
                _settings->setAppUser(value);
            }

            if(_server->argName(i) == "authpass" && _server->arg(i) != "") {
                _settings->setAppPass(value);
            }

            if(_server->argName(i) == "ssid") {
                _settings->setSSID(value);    
            }

            if(_server->argName(i) == "ssidpass") {
                _settings->setSSIDPass(value);
            }

            if(_server->argName(i) == "ifttturl" && _server->arg(i) != "") {
                _settings->setIFTTTKey(value);
            }

            if(_server->argName(i) == "treshold") {
                _settings->setTreshold(value.toInt());
            } 

            if(_server->argName(i) == "tresholdMin") {
                _settings->setTresholdMin(value.toInt());
            } 

            if(_server->argName(i) == "davice_name") {
                _settings->setDeviceName(value);      
            }     
        
        }

        _settings->save();

    }

    String content = "<html><head><style>input,button { font-size: 2em; width: 100%; }</style></head><body><form method=\"post\">";
    content += "<input name=\"authuser\" value=\""+ String(_settings->getAppUser()) +"\"><br/>";
    content += "<input name=\"authpass\" type=\"password\"><br/>";
    content += "<input name=\"ssid\" value=\""+ String(_settings->getSSID()) +"\" ><br/>";
    content += "<input name=\"ssidpass\" value=\""+ String(_settings->getSSIDPass()) +"\"><br/>";
    content += "<input name=\"ifttturl\" value=\""+ String(_settings->getIFTTTKey()) +"\">(Last State: " + String(state)+ ")<br/>";
    content += "<input name=\"treshold\" value=\""+ String(_settings->getTreshold()) +"\"><br/><br/>";
    content += "<input name=\"tresholdMin\" value=\""+ String(_settings->getTresholdMin()) +"\"><br/><br/>";
    content += "<input name=\"davice_name\" value=\""+ String(_settings->getDeviceName()) +"\"><br/><br/>";
    content += "<button type=submit>Save</button></form></body></html>"; 

    _server->send(200, "text/html", content);

}