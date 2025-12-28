cmake_minimum_required(VERSION 3.14)

# 1. Cross-Compilation Setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Force CMake to skip the Linker check (Crucial for Bare Metal)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 2. Point to the Toolchain
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")

project(TivaC_Blinky C ASM)

# 3. Compiler Flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} -g -O0 -Wall")

# 4. The Executable
add_executable(blinky.elf main.c startup.c)

# 5. Linker Options (The Fix)
# We use target_link_options to safely pass the script
target_link_options(blinky.elf PRIVATE
    -T "${CMAKE_SOURCE_DIR}/tm4c123gh6pm.ld"
    -nostartfiles
    --specs=nosys.specs
    -Wl,-Map=blinky.map
)