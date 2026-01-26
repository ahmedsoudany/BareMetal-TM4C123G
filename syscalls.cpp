#include <sys/stat.h>
#include <sys/types.h>
#include "drivers/mcal/UART0.h"

// 1. Reference to the UART driver (defined in main.cpp)
extern UART0 uart;

extern "C" {

    // 2. The Linker Symbol for the end of RAM
    extern char end; 
    static char *heap_end = 0;

    // 3. The Heap Allocator (Critical for printf)
    caddr_t _sbrk(int incr) {
        char *prev_heap_end;

        if (heap_end == 0) {
            heap_end = &end;
        }
        prev_heap_end = heap_end;
        
        // Simple check: Don't blow up the stack
        // (In a real driver, we would check against the Stack Pointer)
        
        heap_end += incr;
        return (caddr_t) prev_heap_end;
    }

    // 4. The Write Function (Retargets printf to UART)
    int _write(int file, char *ptr, int len) {
        // Send to UART
        for (int i = 0; i < len; i++) {
            if (ptr[i] == '\n') {
                uart.writeChar('\r');
            }
            uart.writeChar(ptr[i]);
        }
        return len;
    }

    // 5. Dummy Stubs (To satisfy the Linker)
    int _close(int file) { return -1; }
    int _fstat(int file, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
    int _isatty(int file) { return 1; }
    int _lseek(int file, int ptr, int dir) { return 0; }
    int _read(int file, char *ptr, int len) { return 0; }
    void _exit(int status) { while(1); }
    int _kill(int pid, int sig) { return -1; }
    int _getpid(void) { return 1; }
}