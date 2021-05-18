#include <FS.h>
#include <FFat.h>

void readSettings(const char * path, String *settings) {
  Serial.printf("Reading file: %s\n", path);
  String result = "";

  File file = FFat.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  int i=0;
  while (file.available() && i<6) {
    char x = file.read();
    Serial.print(x);
    if(x == '\n'){
     *(settings++);
     i++;
    } else {  
     settings += x;
    }
  }
  file.close();

}

void writeSettings(const char * path, String *settings) {
  
  Serial.printf("Writing file: %s\n", path);

  File file = FFat.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

Serial.println("Will write");
  int i;
  for(i=0; i < 6; i++) {
    Serial.println(i);
    Serial.println(*settings);
    file.print(*settings);
    file.print('\n');
    *(settings++);
  }
  
  delay(2000); // Make sure the CREATE and LASTWRITE times are different
  file.close();
}

void initSettings(String *settings) {

  Serial.println(*settings);

  if (!FFat.begin()) {
    FFat.format();
    Serial.println("LittleFS mount failed");
    delay(1000);
  }

  if (!FFat.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  if(!FFat.exists("/settings.txt")) {
    writeSettings("/settings.txt", settings);
  }

  readSettings("/settings.txt", settings);
   
}