#ifndef MAIN_HPP
#define MAIN_HPP
#include <esp_littlefs.h> 

 
void confgiureWiFi();
void connectToWifi(bool wifiFound);
void configureDisplay();
void configureFS();
void wifiTask(void* pvParameters);
void dcfTask(void* pvParameters);
void listDir(fs::FS fs, const char * dirname, uint8_t levels);
void setup();
void loop();

#endif