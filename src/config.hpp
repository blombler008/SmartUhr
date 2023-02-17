#include <Arduino.h>  
#include <pins_arduino.h>
#include "dcf77.hpp"
#include "display.hpp" 

// #include <MCP23017.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Wire.h>  
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include <U8x8lib.h>

#include <WiFi.h> 
// #include <LuaWrapper.h>
#include <LittleFS.h>

#ifndef CONFIG_HPP
#define CONFIG_HPP


// Constants 
#define WIFI_SSID "Wickinger"
#define WIFI_HOSTNAME "ESP32.local"
#define WIFI_PASSWORD "EJYBmZ3pRdkHu614B8DC" // secret

// serial
#define SERIAL_BAUD 115200

// display
#define DISPLAY_ADRESS 0x3C
#define OLED_ROTATION U8G2_R2

// dcf
#define DCF77PIN 5 

//  

#define DEBUG
 
#include "PrintHelper.hpp"

#endif