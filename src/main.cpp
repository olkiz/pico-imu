#include "pico/stdlib.h"
#include "hardware/spi.h"

#include <stdio.h>
#include <math.h>

#include "MPU9250.h"
#include "SpiUtils.h"
#include "IMU.h"
#include "Quaternion.h"


#define RAD_TO_DEG 180.0 / M_PI

int main()
{
    const auto CS_PIN = 5;
    const auto CLOCK = 500000;
    const auto MISO_PIN = 4;
    const auto MOSI_PIN = 7;
    const auto SCK_PIN = 6;

    spi_init(spi0, CLOCK, CS_PIN, MISO_PIN, MOSI_PIN, SCK_PIN);

    auto mpu9250 = MPU9250(spi0, CS_PIN);

    stdio_init_all();
    
    int16_t temperature;
    int16_t mag[3];

    double gyro[3];
    double acceleration[3];

    uint64_t prev_time = time_us_64();

    IMU imu{Quaternion(euler_angles{0.0,0.0,0.0})};

    while (true)
    {
        uint64_t current_time = time_us_64();
        double dt = (current_time - prev_time) / 1000000.0;
        prev_time = current_time;
        mpu9250.readAccelerometer(acceleration);
        mpu9250.readGyro(gyro);
        mpu9250.readMagRaw(mag);
        temperature = mpu9250.temperature();
        imu.update(gyro, dt);
        auto angles = imu.orientation().toEuler();
        

        printf("Gyro: %05.2f, %05.2f, %05.2f, acceleration = %05.2f, %05.2f, %05.2f, angles = %05.2f, %05.2f, %05.2f, mag = %06d, %06d, %06d\n",
               gyro[0] * RAD_TO_DEG, gyro[1] * RAD_TO_DEG, gyro[2] * RAD_TO_DEG,
               acceleration[0], acceleration[1], acceleration[2],
               angles.pitch * RAD_TO_DEG, angles.roll * RAD_TO_DEG, angles.yaw * RAD_TO_DEG, 
               mag[0], mag[1], mag[2]
            );
    }
}