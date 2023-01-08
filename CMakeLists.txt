cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(pico-imu)

pico_sdk_init()

add_executable(pico-imu
        main.c
        )

target_link_libraries(pico-imu pico_stdlib)

pico_add_extra_outputs(pico-imu)