#ifndef GAME_H  
#define GAME_H  


#include <stdio.h>
#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"

//Definition for the Deadzone in the Joystick
#define DEADZONELOWERSIDE 1000
#define DEADZONEHIGHERSIDE 3000

//Alligns the chracter left up
#define CHARACTER_PADDING_LEFT_UP 0
#define CHARACTER_PADDING_DOWN_RIGHT 7 
#define DASH_MOV 3

//basic struct for each player
struct player{

    uint8_t x_pos;
    uint8_t y_pos;
    bool alive;

};

void player_move(uint16_t x_mov, uint16_t y_mov, struct player *p1, bool dash, bool stop);
void rand_move(struct player *enemy);
uint8_t rand_spawn(char dir);
void player_shoot(struct player *p1, struct player *enemy1, struct player *enemy2, bool down, bool right);




#endif 