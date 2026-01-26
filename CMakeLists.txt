cmake_minimum_required(VERSION 3.14)

# 1. Cross-Compilation Setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Force CMake to skip the Linker check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 2. Point to the Toolchain
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++") # Required for C++
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")

# [CHANGE 1] Add CXX to enable C++ Language Support
project(TivaC_Blinky C CXX ASM)

# 3. Compiler Flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# C Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} -g -O0 -Wall")

# [CHANGE 2] C++ Specific Flags (Critical for Embedded)
# -fno-rtti: Disable Run-Time Type Information (Saves Flash/RAM)
# -fno-exceptions: Disable try/catch (Saves massive overhead)
# -std=c++14: Use modern C++ features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPU_FLAGS} -g -O0 -Wall -fno-rtti -fno-exceptions -std=c++14")

# [CHANGE 3] Include Directories
# Add drivers/mcal so the compiler can find UART0.h
include_directories(inc drivers/mcal)

# [CHANGE 4] The Executable Sources
# 1. Changed main.c -> main.cpp
# 2. Added drivers/mcal/UART0.cpp
add_executable(blinky.elf main.cpp startup.c syscalls.cpp inc/drivers/mcal/UART0.cpp
)

# 5. Linker Options
target_link_options(blinky.elf PRIVATE
    -T "${CMAKE_SOURCE_DIR}/tm4c123gh6pm.ld"
    -nostartfiles
    --specs=nano.specs
    -Wl,-Map=blinky.map
    -u _printf_float     
)