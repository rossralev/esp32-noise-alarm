#include <WebServer.h>

WebServer server(80);

String *localSettings;

char* strToChars(String str) { 
    // Length (with one extra character for the null terminator)
    int str_len = str.length() + 1; 
    char char_array[str_len];
    str.toCharArray(char_array, str_len);

    return char_array;
}

void handle_http_not_found() {
    server.send(404, "text/plain", "Not Found");
}

void handle_http_settings() {
    Serial.println("Enter Settings");

    Serial.println(*(localSettings));
    Serial.println(*(localSettings+1));
    Serial.println(strToChars(*(localSettings)));
    Serial.println(strToChars(*(localSettings+1)));

    if (!server.authenticate(strToChars(*(localSettings)), strToChars(*(localSettings+1)))) {
        return server.requestAuthentication();
    }    

    if(server.method() == HTTP_POST) {
        for (uint8_t i = 0; i < server.args(); i++) {
            if(server.argName(i) == "authuser")
                *(localSettings) = server.arg(i);

            if(server.argName(i) == "authpass" && server.arg(i) != "") 
                *(localSettings+1) = server.arg(i);

            if(server.argName(i) == "ssid")
                *(localSettings+2) = server.arg(i);    

            if(server.argName(i) == "ssidpass")
                *(localSettings+3) = server.arg(i);

            if(server.argName(i) == "ifttturl")
                *(localSettings+4) = server.arg(i);

            if(server.argName(i) == "treshold")
                *(localSettings+5) = server.arg(i);        
        
        }

        writeSettings("settings.txt", localSettings);

    }

    String content = "<html><body><form method=\"post\">";
    content += "<input name=\"authuser\" value=\""+ *(localSettings) +"\">";
    content += "<input name=\"authpass\" type=\"password\">";
    content += "<input name=\"ssid\" value=\""+ *(localSettings+2) +"\" >";
    content += "<input name=\"ssidpass\" value=\""+ *(localSettings+3) +"\">";
    content += "<input name=\"ifttturl\" value=\""+ *(localSettings+4) +"\">";
    content += "<input name=\"treshold\" value=\""+ *(localSettings+5) +"\">";
    content += "<button type=submit>Save</button></form></body></html>"; 

    server.send(200, "text/html", content);

}

void handle_http_root() {
    Serial.println("Enter handleRoot");

    if (!server.authenticate(strToChars(*(localSettings)), strToChars(*(localSettings+1)))) {
    return server.requestAuthentication();
    }

    String content = "<html><body><H2>hello, you successfully connected to esp8266!</H2><br>";
    if (server.hasHeader("User-Agent")) {
        content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
    }
    content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
    server.send(200, "text/html", content);
}

void serverHandlerInit(String *settings) {
    localSettings = settings;
    server.onNotFound(handle_http_not_found);
    server.on("/", handle_http_root);
    server.on("/settings", handle_http_settings);
    server.begin();
}
