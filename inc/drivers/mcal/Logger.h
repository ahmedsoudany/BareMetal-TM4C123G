#ifndef LOGGER_H
#define LOGGER_H

#include "UART0.h"
#include <cstdio>
#include <cstdarg> // For va_list

class Logger {
public:
    static void log(const char* format, ...) {
        char buffer[128]; // Stack buffer (Safe for single thread)
        
        // 1. Capture the arguments
        va_list args;
        va_start(args, format);
        
        // 2. Format into buffer (Safe vsnprintf)
        std::vsnprintf(buffer, sizeof(buffer), format, args);
        
        // 3. Cleanup
        va_end(args);
        
        // 4. Send to UART (Assuming we have a global instance or singleton)
        // For now, we'll use a raw pointer or extern. 
        // Ideally, pass UART reference in init.
        extern UART0 uart; 
        uart.writeString(buffer);
    }
};

#endif