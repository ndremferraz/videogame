#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#define GREENBUTTON 11
#define YELLOWBUTTON 12
#define REDBUTTON 13
#define BLUEBUTTON 9

#define XJOYSTICK 27
#define YJOYSTICK 26

#define SCREEN_ADDRESS 61

void init_gpios();

void i2c_setup();

int main()
{
    stdio_init_all();
    init_gpios();
    i2c_setup();
    adc_init();

    adc_gpio_init(XJOYSTICK);
    adc_gpio_init(YJOYSTICK);
    adc_set_round_robin(3);

    bool a_pressed, b_pressed, c_pressed, d_pressed = false;
    uint16_t x_read, y_read = 0;

    while (true) {
        a_pressed = gpio_get(GREENBUTTON);
        b_pressed = gpio_get(YELLOWBUTTON);
        c_pressed = gpio_get(REDBUTTON);
        d_pressed = gpio_get(BLUEBUTTON);

        x_read = adc_read();
        y_read = adc_read();

        int ret;
        uint8_t rxdata;

        ret = i2c_read_blocking(i2c_default, SCREEN_ADDRESS, &rxdata, 1, false);

        ret > 0 ? printf("I2C Display found") : printf("I2C Display not found");
        printf("GREEN:%d YELLOW:%d RED:%d BLUE:%d X:%d Y:%d\n", a_pressed, b_pressed, c_pressed, d_pressed, x_read, y_read);
        sleep_ms(2000);
    }
}



void init_gpios(){

    gpio_init(GREENBUTTON);
    gpio_init(YELLOWBUTTON);
    gpio_init(REDBUTTON);
    gpio_init(BLUEBUTTON);
    

    gpio_set_dir(GREENBUTTON, GPIO_IN);
    gpio_set_dir(YELLOWBUTTON, GPIO_IN);
    gpio_set_dir(REDBUTTON, GPIO_IN);
    gpio_set_dir(BLUEBUTTON, GPIO_IN);

}

void i2c_setup(){

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
}