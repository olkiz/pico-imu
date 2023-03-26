#ifndef MPU9250_H
#define MPU9250_H

#include "hardware/spi.h"

struct mpu9250
{
    int clock;
    spi_inst_t *spi_port;
    int cs_pin;
    int miso_pin;
    int mosi_pin;
    int sck_pin;
    uint8_t calibration_data[3];
};

void mpu_init(struct mpu9250 *chip);
void mpu_reset(struct mpu9250 *chip);
void mpu_mag_init(struct mpu9250 *chip, uint8_t scale, uint8_t mode);
void mpu_read_acc_raw(struct mpu9250 *chip, int16_t acc[3]);
void mpu_read_gyro_raw(struct mpu9250 *chip, int16_t gyro[3]);
void mpu_read_mag_raw(struct mpu9250 *chip, int16_t mag[3]);
void mpu_read_gyro_rad_per_sec(struct mpu9250 *chip, double gyro[3]);
int mpu_get_temperature(struct mpu9250 *chip);

#endif // MPU9250_H