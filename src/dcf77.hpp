#ifndef DCF77_HPP
#define DCF77_HPP

#include "display.hpp"

struct DCF77Buffer {  
    uint64_t PF             :1;	// parity year
    uint64_t ParityYear     :1;	// parity year
    uint64_t Year	        :8;	// year (5 -> 2005)
    uint64_t Month	        :5;	// month
    uint64_t Weekday	    :3;	// day of week
    uint64_t Day	        :6;	// day
    uint64_t ParityHour     :1;	// parity hours
    uint64_t Hour	        :6;	// hours
    uint64_t ParityMin      :1;	// parity minutes
    uint64_t Min	        :7;	// minutes
    uint64_t prefix	        :27; 
};
 
class DCF77 {
    public:
        int signalPin; 
        int second;
        int minute;
        int hour;
        uint64_t timeCode;
        int64_t startTime;
        int64_t endTime;
        int buffer;
        int reset;
        Display* display;
        struct DCF77Buffer *bufferBits;
        void (*onUpdate)(DCF77* dcf){ };
        int getHour();
        int getMinute();
        int getSecond();
        void finalizeBuffer();
        void check(); 
        DCF77(int signalPin); 
        void scanSignal(int signal); 
};
unsigned int reverseBits(unsigned int num, int len);
bool isTime(struct DCF77Buffer *bufferBits);
void printBits(uint64_t bits, int len);  
int calcMinute(byte bufferTime);
int calcHour(byte bufferTime);

#endif