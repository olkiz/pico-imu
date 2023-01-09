cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(pico-imu)

pico_sdk_init()

include(src/SourcesList.cmake)

add_executable(pico-imu ${PICO_IMU_SOURCES})

target_include_directories(pico-imu PRIVATE
  src
)

target_link_libraries(pico-imu pico_stdlib hardware_spi)

pico_enable_stdio_usb(pico-imu 1)
pico_enable_stdio_uart(pico-imu 0)
pico_add_extra_outputs(pico-imu)