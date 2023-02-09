#include "config.hpp"
#include <esp_littlefs.h> 
//#include <LuaWrapper.h> 

// Adafruit_NeoPixel ledBuiltin(1, 48, NEO_GRB + NEO_KHZ800);
  

// bool invert = false; 
// int period = 5;
// double steps = ((.2)*(128+64))/period;
// double estimatedSteps = (256+128)/steps;

// void set_led_builtin_color(int color) {
//     ledBuiltin.setPixelColor(0, ledBuiltin.gamma32(ledBuiltin.ColorHSV(color)));
//     ledBuiltin.show();
// }


int minute = 0, hour = 0;
void confgiureWiFi();
void connectToWifi(bool wifiFound);
void configureDisplay();
void configureLUA();

Display disp;
DCF77 dcf(DCF77PIN); 
TaskHandle_t wifiHandle;
TaskHandle_t dcfHandle;

void wifiTask(void* pvParameters) { 
    while (true) {  
        vTaskDelay(20);
    }
}
void dcfTask(void* pvParameters) {
    while (1) { 
        dcf.check();
        vTaskDelay(20); 
    }
}
 
void configureDisplay() {  
    Serial.println("Configuring Display");
    dcf.display = (Display*)&disp;
    disp.begin();
    Serial.printf("Setting display i2c adress: 0x%X\n", DISPLAY_ADRESS);
    xTaskCreate(dcfTask, "dcf", 4096, NULL, 1, &dcfHandle);
}
 
void confgiureWiFi() {  
    Serial.println("Configuring Wifi"); 
    int n = WiFi.scanNetworks(false, true); 
    bool wifiFound = false;
    Serial.println("Number of networks found: " + String(n));
    for (int i = 0; i < n; i++) {
        String wifiName = WiFi.SSID(i);
        Serial.println(" - SSID: " + wifiName); 
        if(wifiName == WIFI_SSID) {
            wifiFound = true;
        }
    }
    Serial.println("------------------------------------"); 
    WiFi.disconnect(true, true);
    WiFi.persistent(false);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE); 
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);  
    if (!WiFi.setHostname("ESP-Cocktailmixer.local")) {
        Serial.println("Hostname failed to configure");
    } 
    connectToWifi(wifiFound);   
    Serial.println("Hostname: " + String(WiFi.getHostname())); 
    //xTaskCreatePinnedToCore(wifiTask, "wifi", 10000, NULL, 2, &wifiHandle, 1);
}

void connectToWifi(bool wifiFound) {
    if(wifiFound) { 
        WiFi.setAutoReconnect(true);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
        
        int timeout = 3000;
        int currtimeout = 0;
        Serial.print("Connecting .");
        while (!WiFi.isConnected()) {
            delay(500); 
            currtimeout+=500;
            Serial.print(".");
            if(timeout < currtimeout) { 
                break;
            }
        }
        Serial.print(" ");
        if(WiFi.status() == WL_CONNECTED) { 
            Serial.println("connected"); 
            Serial.println("WifiIP: "+ WiFi.localIP().toString()); 
            return;
        }
        Serial.println("Failed");
        return;
    }
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Access Point (AP) Mode"); 
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void configureLUA() {
    // esp_littlefs_format("spiffs");
    LittleFS.begin(false, "/littlefs", 10, "spiffs");
    bool mounted = esp_littlefs_mounted("spiffs"); 
    if(!mounted) return;
    Serial.println("Filesystem Mounted!");
    listDir(LittleFS, "/", 0);
    File f = LittleFS.open("/test.lua", "w");
    f.printf("test system\n");
    f.flush(); 
    f.close();
    f = LittleFS.open("/test.lua", "r"); 
    String data = f.readStringUntil('\n');
    Serial.println("FILE: ");
    Serial.print(" - Size: ");
    Serial.println(f.size(), DEC);
    Serial.print(" - Data: \"");
    Serial.print(data);
    Serial.println("\"");
    f.close();
}

void setup() {    
  
    Serial.begin(SERIAL_BAUD);
    Serial.println();
    Serial.println("Starting Arduino");  
    pinMode(DCF77PIN, INPUT); 

    confgiureWiFi();
    configureDisplay(); 
    configureLUA();
  // ledBuiltin.begin();
  // ledBuiltin.show();
  // ledBuiltin.setBrightness(100);
  // ledBuiltin.setPixelColor(0, 255, 255, 255);
  // ledBuiltin.show();
  // printf("steps: %f\n", steps);
  // printf("estimated steps needed: %f\n", estimatedSteps );
} 

// int hue = 0;
// int stepsTook = 0;
// int huePerStep = (65536L / estimatedSteps); 
  
void loop() {
   
    vTaskDelay(0); 
    // display.setDrawColor(invert ? 0 : 1);
    // 
    // for(int i=0; i<=128; i+=steps) {
    //   display.drawTriangle(64,32,i,0,i+steps,0);
    //   display.sendBuffer(); 
    //   hue+=huePerStep;
    //   stepsTook++;
    //   set_led_builtin_color(hue); 
    // }
    // for(int i=0; i<=64; i+=steps) { 
    //   display.drawTriangle(64,32,128,i,128,i+steps);
    //   display.sendBuffer(); 
    //   hue+=huePerStep;
    //   stepsTook++;
    //   set_led_builtin_color(hue); 
    // }
    // for(int i=128; i>=0; i-=steps) {
    //   display.drawTriangle(64,32,i,64,i-steps,64);
    //   display.sendBuffer(); 
    //   hue+=huePerStep;
    //   stepsTook++;
    //   set_led_builtin_color(hue); 
    // }
    // for(int i=64; i>=0; i-=steps) {
    //   display.drawTriangle(64,32,0,i,0,i-steps); 
    //   display.sendBuffer();
    //   hue+=huePerStep;
    //   stepsTook++;
    //   set_led_builtin_color(hue); 
    // } 
    // hue = hue % 65536L;
    // printf("hue: %i,\ttook steps: %i\n", hue, stepsTook);
    // stepsTook = 0;
    // invert =! invert;
    // Serial.print("Invertet: "); 
    // Serial.println(invert);

} 
 