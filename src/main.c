#include "pico/stdlib.h"
#include "hardware/spi.h"

#include <stdio.h>

#include "mpu9250.h"

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
    
    int16_t acc[3], gyro[3], temperature;
    while (true)
    {
        mpu_read_acc_raw(&imu, acc);
        mpu_read_gyro_raw(&imu, gyro);
        temperature = mpu_get_temperature(&imu);

        printf("acc: %d, %d, %d, gyro: %d, %d, %d, temperature: %d\n", acc[0], acc[1], acc[2],
               gyro[0], gyro[1], gyro[2], temperature);
        sleep_ms(100);
    }
}