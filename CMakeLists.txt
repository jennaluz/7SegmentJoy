cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD adafruit_feather_rp2040)
include(pico_sdk_import.cmake)

project(rtos_try_again)

pico_sdk_init()

#add_subdirectory(FreeRTOS)
#add_subdirectory(ProjectFiles)

set(PICO_SDK_FREERTOS_SOURCE /Users/jenna-luzpura/Desktop/Dev/Courses/S23/CS452/pico/FreeRTOS-Kernel)

add_library(freertos
    ${PICO_SDK_FREERTOS_SOURCE}/event_groups.c
    ${PICO_SDK_FREERTOS_SOURCE}/list.c
    ${PICO_SDK_FREERTOS_SOURCE}/queue.c
    ${PICO_SDK_FREERTOS_SOURCE}/stream_buffer.c
    ${PICO_SDK_FREERTOS_SOURCE}/tasks.c
    ${PICO_SDK_FREERTOS_SOURCE}/timers.c
    ${PICO_SDK_FREERTOS_SOURCE}/portable/MemMang/heap_3.c
    ${PICO_SDK_FREERTOS_SOURCE}/portable/GCC/ARM_CM0/port.c
)

target_include_directories(freertos PUBLIC
    .
    ${PICO_SDK_FREERTOS_SOURCE}/include
    ${PICO_SDK_FREERTOS_SOURCE}/portable/GCC/ARM_CM0
)


add_executable(7SegmentJoy
    7SegmentJoy.c
)

pico_enable_stdio_usb(7SegmentJoy 1)
pico_enable_stdio_uart(7SegmentJoy 1)

target_link_libraries(7SegmentJoy pico_stdlib freertos)
pico_add_extra_outputs(7SegmentJoy)
