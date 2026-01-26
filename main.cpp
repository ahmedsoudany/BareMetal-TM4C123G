#include "drivers/mcal/UART0.h"
#include "drivers/mcal/Logger.h"

UART0 uart;

extern "C" int main(void) {
    
    uart.init();
    
    Logger::log("System Boot: OK\r\n");
    Logger::log("Engine Temp: %d C\r\n", 95);
    Logger::log("Battery: %d%%\r\n", 100);

    while(1);
}