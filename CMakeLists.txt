cmake_minimum_required(VERSION 3.13)

# Project name
project(NonContactFlowMeter C)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Include header files
include_directories(include)

# Add source files
add_executable(NonContactFlowMeter
    src/main.c
    src/ultrasonic.c
    src/correlation.c
)
