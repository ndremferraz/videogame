#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define ABUTTON 21
#define BBUTTON 20
#define CBUTTON 19
#define DBUTTON 18


int main()
{
    stdio_init_all();

    gpio_init(ABUTTON);
    gpio_init(BBUTTON);
    gpio_init(CBUTTON);
    gpio_init(DBUTTON);

    gpio_set_dir(ABUTTON, GPIO_IN);
    gpio_set_dir(BBUTTON, GPIO_IN);
    gpio_set_dir(CBUTTON, GPIO_IN);
    gpio_set_dir(DBUTTON, GPIO_IN);

    bool a_pressed, b_pressed, c_pressed, d_pressed = false;

    while (true) {
        a_pressed = gpio_get(ABUTTON);
        b_pressed = gpio_get(BBUTTON);
        c_pressed = gpio_get(CBUTTON);
        d_pressed = gpio_get(DBUTTON);


        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
