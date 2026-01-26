#ifndef UART0_H
#define UART0_H


#include <cstdint>


class UART0 {
public:
	// Constructor
	UART0() = default;

	// SAFETY" Delete Copy Constructor & Assignment Operator
	// Why? A hardware driver represents a physical resource.
	// You cannot have two software objects controlling the same hardware register
	// without synchronization issues.
	UART0(const UART0&) = delete;
	UART0& operator = (const UART0&) = delete;


	// Initialization Sequence
	void init();

	// Core Communication
	void writeChar(char C);
	void writeString(const char* str);



private:
	// Helper to wait until UART is not bust (optional, but good for safety)
	// bool isBusy();
};

#endif // UART0_H