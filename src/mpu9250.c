#include "mpu9250.h"
#include "mpu9250_regs.h"

#include "pico/stdlib.h"

void mpu_init(struct mpu9250 *chip)
{
    spi_init(chip->spi_port, chip->clock);
    gpio_set_function(chip->miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(chip->sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(chip->mosi_pin, GPIO_FUNC_SPI);

    gpio_init(chip->cs_pin);
    gpio_set_dir(chip->cs_pin, GPIO_OUT);
    gpio_put(chip->cs_pin, 1);
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

void mpu_read_acc_raw(struct mpu9250 *chip, int16_t acc[3])
{
    uint8_t buf[6];

    mpu_read(chip, ACCEL_XOUT_H, buf, 6);

    for (int i = 0; i < 3; i++)
    {
        acc[i] = (buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }
}

void mpu_read_gyro_raw(struct mpu9250 *chip, int16_t gyro[3])
{
    uint8_t buf[6];

    mpu_read(chip, GYRO_XOUT_H, buf, 6);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = (buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }
}

int mpu_get_temperature(struct mpu9250 *chip)
{
    uint8_t buf[2];
    mpu_read(chip, TEMP_OUT_H, buf, 2);
    return buf[0] << 8 | buf[1];
}
