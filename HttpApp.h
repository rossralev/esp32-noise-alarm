#ifndef HttpApp_h
#define HttpApp_h

#include <WebServer.h>
#include "Settings.h"
#include "Arduino.h"

class HttpApp{
    public:
        HttpApp(Settings *settings);
        void handle_http_not_found();
        void handle_http_settings();
        void handle_http_root();
        void handle();
        int state = 0;
    private:
        Settings *_settings;    
        WebServer *_server;
};

#endif