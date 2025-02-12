#include "game.h"
#include "ssd1306.h"

//function that reads joystick movement and moves player accordingly
//if dash button is pressed the player will move 3 pixels at once instead of 1
void player_move(uint16_t x_mov, uint16_t y_mov, struct player *p1, bool dash, bool stop){
    if(!stop){
        //dash_multiplier is used to control the dash movement and limit bounds accordingly
        uint8_t dash_multiplier = dash ? DASH_MOV : 1; 

        if(x_mov < DEADZONELOWERSIDE){

            if(p1->x_pos - dash_multiplier >= CHARACTER_PADDING_LEFT_UP){
                p1->x_pos -= dash_multiplier;
            }

        } 
        if(x_mov > DEADZONEHIGHERSIDE) {
            if (p1->x_pos + dash_multiplier < SSD1306_WIDTH - CHARACTER_PADDING_DOWN_RIGHT){
                p1->x_pos += dash_multiplier;
            }
            
        
        }

        if(y_mov > DEADZONELOWERSIDE){

            if(p1->y_pos - dash_multiplier >= CHARACTER_PADDING_LEFT_UP){
                p1->y_pos -= dash_multiplier;
            }

        } 
        if(y_mov < DEADZONEHIGHERSIDE) {
            if (p1->y_pos + dash_multiplier < SSD1306_HEIGHT - CHARACTER_PADDING_DOWN_RIGHT){
                p1->y_pos += dash_multiplier;
            }
        
        }
        
    }
    
    
    
}

//uses random number generator to determine in what direction the enemy will move 
void rand_move(struct player *enemy){
    uint8_t dir = (uint8_t)(get_rand_32() % 4);
    switch (dir){
        case 0:
            if (enemy->x_pos - DASH_MOV >= CHARACTER_PADDING_LEFT_UP)
            {
                enemy->x_pos -= DASH_MOV;
            }
            break;
        case 1:
            if (enemy->y_pos - DASH_MOV >= CHARACTER_PADDING_LEFT_UP)
            {
                enemy->y_pos -= DASH_MOV;
            }
            break;
        case 2:
            if (enemy->x_pos + DASH_MOV < SSD1306_WIDTH - CHARACTER_PADDING_DOWN_RIGHT)
            {
                enemy->x_pos += DASH_MOV;
            }
            break;
        case 3:
            if (enemy->y_pos + DASH_MOV < SSD1306_HEIGHT - CHARACTER_PADDING_DOWN_RIGHT)
            {
                enemy->y_pos += DASH_MOV;
            }
        break;
        
        default:
            break;
    }
} 


//randomizes the spawn position for the enemies
uint8_t rand_spawn(char dir){

    uint8_t max = dir == 'x' ? SSD1306_WIDTH : SSD1306_HEIGHT;
     
    //generates spawn location from 0 to (63 - Padding)  for height
    //generates spawn location from 0 to (127 - Padding) for width  
    return (uint8_t)(get_rand_32() % (max - CHARACTER_PADDING_DOWN_RIGHT) );

}


//checks if the enemy and player are in the same x position
//and if the enemy`s position is to below the player 
static bool is_below(struct player *p1, struct player *enemy){

    return p1->y_pos <= enemy->y_pos && p1->x_pos == enemy->x_pos;

}

//checks if the enemy and player are in the same y postion
//and if the enemy`s position is to the right of players x position  
static bool is_to_right(struct player *p1, struct player *enemy){

    return p1->x_pos <= enemy->x_pos && p1->y_pos == enemy->y_pos;

}

//checks if shoot buttons are pressed and is there are enemy 
//below or to the right respectively
void player_shoot(struct player *p1, struct player *enemy1, struct player *enemy2, bool down, bool right){

    if(down){
        if(is_below(p1,enemy1)){
            enemy1->alive = false;
        }

        if(is_below(p1,enemy2)){
            enemy2->alive = false;
        }
        
    }

    if(right){

        if(is_to_right(p1,enemy1)){
            enemy1->alive = false;
        }

        if(is_to_right(p1,enemy2)){
            enemy2->alive = false;
        }

    }
}

