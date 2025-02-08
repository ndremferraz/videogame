#include <stdio.h>
#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

//pins 10 and 13 are likely burned :)
#define GREENBUTTON 11
#define YELLOWBUTTON 8
#define REDBUTTON 12
#define BLUEBUTTON 9

#define XJOYSTICK 27
#define YJOYSTICK 26

#define DEADZONELOWERSIDE 1000
#define DEADZONEHIGHERSIDE 3000

#define SCREEN_ADDRESS 61

struct player{

    uint8_t x_pos;
    uint8_t y_pos;

};

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


void player_move(uint16_t x_mov, uint16_t y_mov, struct player *p1, bool dash, bool stop){
    if(!stop){
        uint8_t dash_multiplier = dash ? 3 : 1; 

        if(x_mov < DEADZONELOWERSIDE){

            if(p1->x_pos - dash_multiplier >= 2){
                p1->x_pos -= dash_multiplier;
            }

        } 
        if(x_mov > DEADZONEHIGHERSIDE) {

            if (p1->x_pos + dash_multiplier <= 126){
                p1->x_pos += dash_multiplier;
            }
        
        }

        if(y_mov > DEADZONELOWERSIDE){

            if(p1->y_pos - dash_multiplier >= 2){
                p1->y_pos -= dash_multiplier;
            }

        } 
        if(y_mov < DEADZONEHIGHERSIDE) {

            if (p1->y_pos + dash_multiplier < 62){
                p1->y_pos += dash_multiplier;
            }
        
        }
    }
    
    
    
}

void rand_move(struct player *enemy){
    uint8_t dir = (uint8_t)(get_rand_32() % 4);
    switch (dir){
        case 0:
            if (enemy->x_pos > 5)
            {
                enemy->x_pos -= 3;
            }
            break;
        case 1:
            if (enemy->y_pos > 5)
            {
                enemy->y_pos -= 3;
            }
            break;
        case 2:
            if (enemy->x_pos < 59)
            {
                enemy->x_pos += 3;
            }
            break;
        case 3:
            if (enemy->y_pos < 123)
            {
                enemy->y_pos += 3;
            }
            break;
        default:
            break;
    }
} 

uint8_t rand_spawn(char dir){

    uint8_t max = dir == 'x' ? 62 : 126;

    return (uint8_t)(get_rand_32() % (max - 2 + 1) + 2);

}


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

    struct player p1 = {0,0};
    struct player enemy1 = {rand_spawn('x'),rand_spawn('y')};
    struct player enemy2 = {rand_spawn('x'),rand_spawn('y')};

    while (true) {
        a_pressed = gpio_get(GREENBUTTON);
        b_pressed = gpio_get(YELLOWBUTTON);
        c_pressed = gpio_get(REDBUTTON);
        d_pressed = gpio_get(BLUEBUTTON);

        x_read = adc_read();
        y_read = adc_read();

        int ret;
        uint8_t rxdata;

        player_move(x_read, y_read, &p1, d_pressed, b_pressed);
        rand_move(&enemy1);
        rand_move(&enemy2);
        

        ret = i2c_read_blocking(i2c_default, SCREEN_ADDRESS, &rxdata, 1, false);

        ret > 0 ? printf("I2C Display found ") : printf("I2C Display not found ");
        printf("GREEN:%d YELLOW:%d RED:%d BLUE:%d X:%d Y:%d\n", a_pressed, b_pressed, c_pressed, d_pressed, x_read, y_read);
        printf("Player 1 pos: x = %d y = %d\n", p1.x_pos, p1.y_pos);
        printf("Enemy 1 pos: x = %d y = %d\n", enemy1.x_pos, enemy1.y_pos);
        printf("Enemy 2 pos: x = %d y = %d\n", enemy2.x_pos, enemy2.y_pos);
        sleep_ms(500);
    }
}
