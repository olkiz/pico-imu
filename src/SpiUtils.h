#ifndef SPI_UTILS
#define SPI_UTILS

#include "hardware/spi.h"

void spi_init(spi_inst_t *spi_port, int clock, int cs_pin, int miso_pin, int mosi_pin, int sck_pin )
{
    spi_init(spi_port, clock);
    gpio_set_function(miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);

    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);
}

#endif // SPI_UTILS