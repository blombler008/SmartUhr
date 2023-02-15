
#include "config.hpp"
#include "PrintHelper.hpp"
#include "dcf77.hpp"
#include <stdint.h>


const int splitTiming = 140;
void serialDumpTime(DCF77* dcf);
void addMinute(DCF77* dcf);
void addHour(DCF77* dcf);

int DCF77::getHour() {
    return hour;
}

int DCF77::getMinute() { 
    return minute;   
}

int DCF77::getSecond() {
    return second;
}

void DCF77::finalizeBuffer() {  
    void* bufferPtr = malloc(sizeof(struct DCF77Buffer));
    bufferBits = (struct DCF77Buffer *)(uint64_t)bufferPtr;
    memcpy(bufferBits, &timeCode, sizeof(struct DCF77Buffer)); 
    
    addMinute(this);
    if(isTime(bufferBits)) {
        minute = calcMinute(bufferBits->Min);
        hour = calcHour(bufferBits->Hour);
    }
    
    timeCode = 0; 
    second = 0;
    reset = true;
}

void DCF77::check() { 
    int signal = digitalRead(signalPin);
    if (buffer != signal) {
        buffer = signal;
        scanSignal(signal);
    } 
}

void DCF77::scanSignal(int signal) {
    if(!signal) { 
        startTime = esp_timer_get_time(); 
        display->printTimeToDisplay(second, minute, hour); 
        return;
    }

    int64_t newEndTime = esp_timer_get_time();
    int64_t endDiff = (newEndTime - endTime)/1000;
    endTime = newEndTime;
    int64_t diff = (endTime - startTime)/1000;
    int value = (diff >= splitTiming);

    timeCode <<= 1;
    timeCode += value;
 
    second++;
    if(second > 59) {
        second = 0;
        addMinute(this);
    }
    if(endDiff > 1700) {  
        finalizeBuffer(); 
    }   

}

unsigned int reverseBits(unsigned int num, int len) {
    int count = len;
    int reverse_num = 0;   

    for(uint8_t i=0; i<7;++i){
        reverse_num = (reverse_num|((num>>i)&0x01))<<1; 
    }

    reverse_num >>= (8-len);
    return reverse_num;
}

DCF77::DCF77(int signalPin) { 
    DCF77::signalPin = signalPin; 
}
  
void serialDumpTime(DCF77* dcf) {     
    if (dcf->bufferBits) {
        int minuteGot = dcf->bufferBits->Min;
        int hourGot = dcf->bufferBits->Hour;
        Serial.print("Minute: ");  
        Serial.println(minuteGot, BIN);
        Serial.print("Hour: ");  
        Serial.println(hourGot, BIN);
    }

    Serial.print("isTime: ");  
    Serial.println(isTime(dcf->bufferBits) ? "true" : "false");
     
    Serial.print("Bits String: ");  
    printBits(dcf->timeCode, 64); 
    Serial.println();
    Serial.print("Got String: ");  
    Serial.println(dcf->timeCode, BIN);
    Serial.println("reset"); 
}

void printBits(uint64_t bits, int len) {
    int j; 
    for(j = 0; j < len; j++) {  
        if(j%8 == 0 && j!=0) { 
            dprintf(" ", ""); 
        }
        char bit = ((bits >> j) & 1) ? '1' : '0';  
        dprintf("%c", bit); 
        #if DEBUG
            fflush(stdout);
        #endif
    }
}

bool isTime(struct DCF77Buffer *bufferBits) { 
    if(!bufferBits) return false; 

    int prefix = bufferBits->prefix;
    int mask = 0x1; 
    int fStartOfMinute = (mask) & prefix;
    int fStartOfTime = (mask << 20) & prefix;

    bool ret = true;
    if(fStartOfTime > 0 && fStartOfMinute == 0) ret = true;
    return ret;
}
 

int calcHour(byte bufferBits) { 
    int hour = 0;
    int mask = 1;
    int nums[] = {20,10,8,4,2,1};
    for(int i=0; i < 6; i++) {
        if((bufferBits & mask) > 0) {
            hour = hour + nums[i];
        }
        mask = mask << 1;
    }    

    if (hour > 23) return 0;
    return hour;
}

int calcMinute(byte bufferBits) {  
    int minute = 0; 
    int mask = 1;
    int nums[] = {40,20,10,8,4,2,1};
    for(int i=0; i < 7; i++) {
        if((bufferBits & mask) > 0) {
            minute = minute + nums[i];
        }
        mask = mask << 1;
    }  

    if (minute > 59) return 0;
    return minute;
}

void addMinute(DCF77* dcf) {
    int minute = dcf->minute;
    minute++;
    if(minute > 59) {
        minute = 0;
        addHour(dcf);
    }

    dcf->minute = minute;
}

void addHour(DCF77* dcf) {

    int hour = dcf->hour;
    hour++;
    if(hour > 23) {
        hour = 0; 
    }

    dcf->hour = hour;
}
 