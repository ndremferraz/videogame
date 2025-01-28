#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define GREENBUTTON 11
#define YELLOWBUTTON 12
#define REDBUTTON 13
#define BLUEBUTTON 9


int main()
{
    stdio_init_all();

    gpio_init(GREENBUTTON);
    gpio_init(YELLOWBUTTON);
    gpio_init(REDBUTTON);
    gpio_init(BLUEBUTTON);
    

    gpio_set_dir(GREENBUTTON, GPIO_IN);
    gpio_set_dir(YELLOWBUTTON, GPIO_IN);
    gpio_set_dir(REDBUTTON, GPIO_IN);
    gpio_set_dir(BLUEBUTTON, GPIO_IN);

    bool a_pressed, b_pressed, c_pressed, d_pressed = false;

    while (true) {
        a_pressed = gpio_get(GREENBUTTON);
        b_pressed = gpio_get(YELLOWBUTTON);
        c_pressed = gpio_get(REDBUTTON);
        d_pressed = gpio_get(BLUEBUTTON);


        printf("GREEN: %d, YELLOW: %d, RED: %d, BLUE: %d\n", a_pressed, b_pressed, c_pressed, d_pressed);
        sleep_ms(200);
    }
}
