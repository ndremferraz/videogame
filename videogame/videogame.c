#include <stdio.h>
#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "character_images.h"
#include "game.h"

//pins 10 and 13 are likely burned :)
//Definitions for the buttons and joystick
#define GREENBUTTON 11
#define YELLOWBUTTON 8
#define REDBUTTON 12
#define BLUEBUTTON 9

#define XJOYSTICK 27
#define YJOYSTICK 26

//Initializing the I/O pins
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

//initializing i2c using the default i2c in the pico
//DATA > GPIO4 
//CLK > GPIO5
void i2c_setup(){

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
}


//function finds the index of bytes of each character 
//each character has a width of 8 bytes stacked vertically 
//we want one byte adjacent down 
//adjacent bytes are to compensate for misallignment   
void bytes_of_characters(struct player *character, uint8_t *byte_list){
 
    for(uint8_t i = 0; i < 8; i++){
        byte_list[i] = character->x_pos + i + (character->y_pos / 8)* SSD1306_WIDTH; //index of top byte
        byte_list[i+8] = character->x_pos + i + ((character->y_pos / 8) + 1) * SSD1306_WIDTH; //index of bottom byte
    }  

}

//updates the buffer based on character postion given the character image
void update_character_in_buff(struct player *character, uint8_t *buf, uint8_t *img){
    
    //byte_list stores the indexes in the image buffer where the image should go based on character location 
    uint8_t byte_list[16];
    bytes_of_characters(character, byte_list);

    
    for(uint8_t i = 0; i < 8; i++){
        buf[byte_list[i]] |= img[i] << (character->y_pos % 8);
        buf[byte_list[i + 8]] |= img[i] >> (8 - (character->y_pos % 8));            
    }

}


//Updates the screen based on plyaer location 
void render_screen(uint8_t *buf, struct player *p1, 
    struct player *enemy1, struct player *enemy2, struct render_area *frame_area){
    
    //making the buffer all 0s 
    memset(buf, 0, SSD1306_BUF_LEN);

    //updating the player in image buffer
    update_character_in_buff(p1, buf, player_img);

    //checking if enemies are still alive and updating them in the image buffer 
    if(enemy1->alive){
        update_character_in_buff(enemy1, buf, enemy1_img);
    }
    if(enemy2->alive){
        update_character_in_buff(enemy2, buf, enemy2_img);
    }

    render(buf, frame_area);
}


int main()
{
    stdio_init_all();
    init_gpios();
    i2c_setup();
    adc_init();

    SSD1306_init();

    adc_gpio_init(XJOYSTICK);
    adc_gpio_init(YJOYSTICK);
    adc_set_round_robin(3); //Performing sequential ADC sampling in ADC0 and ADC1

    bool a_pressed, b_pressed, c_pressed, d_pressed = false;
    uint16_t x_read, y_read = 0;

    //spawns player at position 2,2 
    //spawns eneymies at random location 
    struct player p1 = {0,0,true};
    struct player enemy1 = {rand_spawn('x'),rand_spawn('y'),true};
    struct player enemy2 = {rand_spawn('x'),rand_spawn('y'),true};

    struct render_area frame_area = {
        start_col: 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
    };

    calc_render_area_buflen(&frame_area);

    // zero the entire display
    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    while (true) {
        
        a_pressed = gpio_get(GREENBUTTON);
        b_pressed = gpio_get(YELLOWBUTTON);
        c_pressed = gpio_get(REDBUTTON);
        d_pressed = gpio_get(BLUEBUTTON);

        x_read = adc_read();
        y_read = adc_read();


        player_move(x_read, y_read, &p1, d_pressed, b_pressed);
        rand_move(&enemy1);
        rand_move(&enemy2);
        player_shoot(&p1, &enemy1, &enemy2,a_pressed,c_pressed);
        


        printf("GREEN:%d YELLOW:%d RED:%d BLUE:%d X:%d Y:%d\n", a_pressed, b_pressed, c_pressed, d_pressed, x_read, y_read);
        printf("Player 1 pos: x = %d y = %d\n", p1.x_pos, p1.y_pos);
        printf("Enemy 1 pos: x = %d y = %d alive: %d\n", enemy1.x_pos, enemy1.y_pos, enemy1.alive);
        printf("Enemy 2 pos: x = %d y = %d alive: %d\n", enemy2.x_pos, enemy2.y_pos, enemy2.alive);
        render_screen(buf, &p1, &enemy1, &enemy2, &frame_area);


        sleep_ms(500);

    }
}
