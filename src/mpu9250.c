#include "mpu9250.h"
#include "mpu9250_regs.h"

#include "pico/stdlib.h"

void mpu_read(struct mpu9250 *chip, uint8_t reg, uint8_t *buf, uint16_t len);
void mpu_write_byte(struct mpu9250 *chip, uint8_t reg, uint8_t byte);
void mpu_mag_init(struct mpu9250 *chip, uint8_t scale, uint8_t mode);
void mpu_slave_write_byte(struct mpu9250 *chip, uint8_t i2c_addr, uint8_t reg, uint8_t byte);
void mpu_slave_prepare_to_read_bytes(struct mpu9250 *chip, uint8_t i2c_addr, uint8_t reg, uint8_t num);

void mpu_init(struct mpu9250 *chip)
{
    spi_init(chip->spi_port, chip->clock);
    gpio_set_function(chip->miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(chip->sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(chip->mosi_pin, GPIO_FUNC_SPI);

    gpio_init(chip->cs_pin);
    gpio_set_dir(chip->cs_pin, GPIO_OUT);
    gpio_put(chip->cs_pin, 1);

    mpu_write_byte(chip, PWR_MGMT_1, 0x00);
    sleep_ms(100);
    mpu_write_byte(chip, PWR_MGMT_1, 0x01);
    sleep_ms(200);

    mpu_write_byte(chip, USER_CTRL, 0x20);
    mpu_write_byte(chip, I2C_MST_CTRL, 0x1D);
    mpu_write_byte(chip, I2C_MST_DELAY_CTRL, 0x81);
    mpu_write_byte(chip, I2C_SLV4_CTRL, 0x01);

    mpu_mag_init(chip, MAG_16_BIT_SCALE, MAG_100HZ_MODE);
}

void mpu_mag_init(struct mpu9250 *chip, uint8_t scale, uint8_t mode)
{
    uint8_t calibration_data[3];
    mpu_slave_write_byte(chip, MAG_ADDRESS, MAG_CNTL2, 0x01); // Mag reset
    sleep_ms(100);
    mpu_slave_write_byte(chip, MAG_ADDRESS, MAG_CNTL1, 0x00); // Mag power down
    sleep_ms(100);
    mpu_slave_write_byte(chip, MAG_ADDRESS, MAG_CNTL1, 0x0F); // Fuze ROM
    sleep_ms(100);

    mpu_slave_prepare_to_read_bytes(chip, MAG_ADDRESS, MAG_ASAX, 3);
    sleep_ms(100);
    mpu_read(chip, EXT_SENS_DATA_00, calibration_data, 3); // Read calibration data

    chip->calibration_data[0] = calibration_data[0];
    chip->calibration_data[1] = calibration_data[1];
    chip->calibration_data[2] = calibration_data[2];

    mpu_slave_write_byte(chip, MAG_ADDRESS, MAG_CNTL1, 0x00); // Mag power down
    sleep_ms(100);
    mpu_slave_write_byte(chip, MAG_ADDRESS, MAG_CNTL1, scale << 4 | mode); // Setting resolution and sample ODR
    sleep_ms(100);
}

void cs_select(struct mpu9250 *chip)
{
    gpio_put(chip->cs_pin, 0);
}

void cs_deselect(struct mpu9250 *chip)
{
    gpio_put(chip->cs_pin, 1);
}

void mpu_reset(struct mpu9250 *chip)
{
    uint8_t buf[] = {PWR_MGMT_1, 0x00};
    cs_select(chip);
    spi_write_blocking(chip->spi_port, buf, 2);
    cs_deselect(chip);
}

void mpu_read(struct mpu9250 *chip, uint8_t reg, uint8_t *buf, uint16_t len)
{
    reg |= MPU_9250_READ_BIT;
    cs_select(chip);
    spi_write_blocking(chip->spi_port, &reg, 1);
    spi_read_blocking(chip->spi_port, 0, buf, len);
    cs_deselect(chip);
}

void mpu_write_byte(struct mpu9250 *chip, uint8_t reg, uint8_t byte)
{
    cs_select(chip);
    spi_write_blocking(chip->spi_port, &reg, 1);
    spi_write_blocking(chip->spi_port, &byte, 1);
    cs_deselect(chip);
}

void mpu_read_acc_raw(struct mpu9250 *chip, int16_t acc[3])
{
    uint8_t buf[6];

    mpu_read(chip, ACCEL_XOUT_H, buf, 6);

    for (int i = 0; i < 3; i++)
    {
        acc[i] = ((int16_t)buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }
}

void mpu_read_gyro_raw(struct mpu9250 *chip, int16_t gyro[3])
{
    uint8_t buf[6];

    mpu_read(chip, GYRO_XOUT_H, buf, 6);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = ((int16_t)buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }
}

void mpu_slave_prepare_to_read_bytes(struct mpu9250 *chip, uint8_t i2c_addr, uint8_t reg, uint8_t num)
{
    mpu_write_byte(chip, I2C_SLV0_ADDR, i2c_addr | MPU_9250_READ_BIT);
    mpu_write_byte(chip, I2C_SLV0_REG, reg);
    mpu_write_byte(chip, I2C_SLV0_CTRL, MPU_9250_ENABLE_I2C | num);
}

void mpu_slave_write_byte(struct mpu9250 *chip, uint8_t i2c_addr, uint8_t reg, uint8_t byte)
{
    mpu_write_byte(chip, I2C_SLV0_ADDR, i2c_addr);
    mpu_write_byte(chip, I2C_SLV0_REG, reg);
    mpu_write_byte(chip, I2C_SLV0_DO, byte);
    mpu_write_byte(chip, I2C_SLV0_CTRL, MPU_9250_ENABLE_I2C | 0x01);
}

void mpu_read_mag_raw(struct mpu9250 *chip, int16_t mag[3])
{
    uint8_t buf[7];
    uint8_t data_ready;
    mpu_slave_prepare_to_read_bytes(chip, MAG_ADDRESS, MAG_ST1, 1);
    sleep_ms(2);
    mpu_read(chip, EXT_SENS_DATA_00, &data_ready, 1);

    mpu_slave_prepare_to_read_bytes(chip, MAG_ADDRESS, MAG_XOUT_L, 7);
    sleep_ms(2);

    mpu_read(chip, EXT_SENS_DATA_00, buf, 7);

    bool is_overflow_set = buf[6] & 0x08;

    if (!is_overflow_set)
    {
        for (int i = 0; i < 3; i++)
        {
            mag[i] = ((int16_t)buf[i * 2 + 1] << 8 | buf[(i * 2)]);
        }
    }
}

void mpu_read_gyro_rad_per_sec(struct mpu9250 *chip, double gyro[3])
{
    int16_t gyro_raw[3];
    mpu_read_gyro_raw(chip, gyro_raw);
    gyro[0] = gyro_raw[0] * 0.000133061;
    gyro[1] = gyro_raw[1] * 0.000133061;
    gyro[2] = gyro_raw[2] * 0.000133061;
}

int mpu_get_temperature(struct mpu9250 *chip)
{
    uint8_t buf[2];
    mpu_read(chip, TEMP_OUT_H, buf, 2);
    return buf[0] << 8 | buf[1];
}
