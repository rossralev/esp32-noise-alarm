// The MIT License
// 
// Copyright (c) 2015 Neil Webber
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// ====================================================================
// ====================================================================
// An adapted version of IFTTT Trigger by Neil Webber
// Original: https://gist.github.com/outofmbufs/d6ced37b49a484c495f0
// 
// Adaptation from: https://www.siytek.com
//
// How to use:
// 1. Place AnotherIFTTTWebhook.h in the same directory as your project
// 2. Add #include "AnotherIFTTTWebhook.h" to your main project file
// 3. Send webhook using function in main file:
//    send_webhook(EVENT, KEY, Value1, Value2, Value3);
// ====================================================================
// ====================================================================

#include <HTTPClient.h>

HTTPClient http;

void send_webhook(String MakerIFTTT_Event, String MakerIFTTT_Key, String value1, String value2, String value3) {

    // connect to the Maker event server
    http.begin("http://maker.ifttt.com/trigger/" + MakerIFTTT_Event + "/with/key/" + MakerIFTTT_Key);
    http.addHeader("Content-Type", "application/json");

    String c = "{\"value1\":\"" + value1 + "\",\"value2\":\"" + value2 + "\",\"value3\":\"" + value3 + "\"}";
    Serial.println(c);

    int httpResponseCode = http.POST(c);

    if(httpResponseCode > 0 ) {
    
        String response = http.getString();  //Get the response to the request
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer
    
    }else{
    
        String response = http.getString();
        Serial.println(response); 

        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    
    }
    
}