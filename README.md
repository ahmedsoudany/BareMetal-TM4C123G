# Bare Metal Blinky - TM4C123G (ARM Cortex-M4)

## Overview
This project demonstrates **Bare Metal Embedded C** programming on the Texas Instruments **TM4C123GH6PM** microcontroller (Tiva C LaunchPad). 

Unlike standard examples that rely on vendor libraries (TivaWare) or Arduino abstractions, this project interacts directly with the hardware. It implements a custom build system, startup code, and linker script from scratch to control GPIO peripherals via **Memory-Mapped I/O**.

## Hardware & Toolchain
*   **Board:** TI Tiva C LaunchPad (EK-TM4C123GXL)
*   **MCU:** TM4C123GH6PM (ARM Cortex-M4F)
*   **Compiler:** `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain)
*   **Build System:** CMake + Ninja
*   **Debugger:** OpenOCD + GDB
*   **IDE:** VS Code

## Key Technical Concepts
This project serves as a proof-of-concept for the following embedded engineering skills:

### 1. Low-Level Hardware Control
*   **Direct Register Access:** Configuring GPIO direction, digital enable, and data registers using pointers to absolute memory addresses.
*   **Bit Manipulation:** Using bitwise operators (`|=`, `&= ~`, `^=`) to modify specific bits without corrupting adjacent configuration.
*   **Clock Gating:** Manually enabling the system clock for peripherals (RCGCGPIO) before access to prevent bus faults.

### 2. Custom Boot Process
*   **Linker Script (`.ld`):** Manually defined memory map to allocate code to **FLASH** (0x00000000) and variables to **SRAM** (0x20000000).
*   **Startup Code (`startup.c`):** Implemented the **Reset Handler** to:
    1.  Copy the `.data` section from Flash to RAM.
    2.  Zero-initialize the `.bss` section.
    3.  Configure the Vector Table and Stack Pointer.
    4.  Jump to `main()`.

### 3. Cross-Compilation Workflow
*   Configured **CMake** to handle the cross-compilation process, passing specific CPU flags (`-mcpu=cortex-m4`, `-mfloat-abi=hard`) and linker flags (`-nostartfiles`, `--specs=nosys.specs`).

## Build & Debug Instructions

### Prerequisites
*   ARM GCC Toolchain
*   OpenOCD
*   CMake & Ninja

### Build
```bash
mkdir build
cd build
cmake -G Ninja ..
cmake --build .