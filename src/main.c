#include "pico/stdlib.h"
#include "hardware/spi.h"

#include <stdio.h>
#include <math.h>

#include "mpu9250.h"
#include "imu.h"
#include "quaternion.h"


#define RAD_TO_DEG 180.0 / M_PI

int main()
{
    struct mpu9250 imu;
    imu.clock = 500000;
    imu.cs_pin = 5;
    imu.miso_pin = 4;
    imu.mosi_pin = 7;
    imu.sck_pin = 6;
    imu.spi_port = spi0;

    mpu_init(&imu);

    mpu_reset(&imu);

    stdio_init_all();
    
    int16_t acc[3], temperature;

    double gyro[3];

    // Initial location
    struct euler_angles angles = {.pitch = 0.1, .roll = 0.05, .yaw = 0.3};

    struct quaternion location = euler_angles_to_quaternion(&angles);
    uint64_t prev_time = time_us_64();
    while (true)
    {
        uint64_t current_time = time_us_64();
        double dt = (current_time - prev_time) / 1000000.0;
        prev_time = current_time;
        mpu_read_acc_raw(&imu, acc);
        mpu_read_gyro_rad_per_sec(&imu, gyro);
        temperature = mpu_get_temperature(&imu);
        struct quaternion quaternion_from_gyro = compute_quaternion_from_gyro(gyro, dt);
        location = mul_quaternion(&location, &quaternion_from_gyro);
        normalize(&location);

        angles = quaternion_to_euler_angles(&location);

        printf("gyro: %f, %f, %f, angles = %f, %f, %f, q = %f, %f, %f, %f\n",
               gyro[0] * RAD_TO_DEG, gyro[1] * RAD_TO_DEG, gyro[2] * RAD_TO_DEG, 
               angles.pitch * RAD_TO_DEG, angles.roll * RAD_TO_DEG, angles.yaw * RAD_TO_DEG, 
               location.a, location.x, location.y, location.z);
    }
}