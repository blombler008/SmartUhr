#ifndef PRINT_HELP_HPP
#define PRINT_HELP_HPP

#ifdef DEBUG
    #define dprintf(...) Serial.printf(__VA_ARGS__);
    #define dprintln(msg) Serial.println(msg);
    #define dprint(msg) Serial.print(msg);
#else
    #define dprintf(...) 
    #define dprintln(msg) 
    #define dprint(msg) 
#endif
#endif