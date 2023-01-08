#include "pico/stdlib.h"
#include <stdio.h>

int main()
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    while (true)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        printf("LED ON\n");
        sleep_ms(100);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        printf("LED OFF\n");
        sleep_ms(100);
    }
}