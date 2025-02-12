#ifndef SSD1306_H  
#define SSD1306_H  

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"


#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128 

#define SSD1306_I2C_ADDR 0x3D


//CLK SPEED DEFINITION FOR I2C
#define SSD1306_I2C_CLK             400

//LIST OF COMMANDS LOOK AT SSD1306 for greater details
#define SSD1306_SET_MEM_MODE        0x20
#define SSD1306_SET_COL_ADDR        0x21
#define SSD1306_SET_PAGE_ADDR       0x22
#define SSD1306_SET_HORIZ_SCROLL    0x26
#define SSD1306_SET_SCROLL          0x2E

#define SSD1306_SET_DISP_START_LINE 0x40

#define SSD1306_SET_CONTRAST        0x81
#define SSD1306_SET_CHARGE_PUMP     0x8D

#define SSD1306_SET_SEG_REMAP       0xA0
#define SSD1306_SET_ENTIRE_ON       0xA4
#define SSD1306_SET_ALL_ON          0xA5
#define SSD1306_SET_NORM_DISP       0xA6
#define SSD1306_SET_INV_DISP        0xA7
#define SSD1306_SET_MUX_RATIO       0xA8
#define SSD1306_SET_DISP            0xAE
#define SSD1306_SET_COM_OUT_DIR     0xC0
#define SSD1306_SET_COM_OUT_DIR_FLIP 0xC0

#define SSD1306_SET_DISP_OFFSET     0xD3
#define SSD1306_SET_DISP_CLK_DIV    0xD5
#define SSD1306_SET_PRECHARGE       0xD9
#define SSD1306_SET_COM_PIN_CFG     0xDA
#define SSD1306_SET_VCOM_DESEL      0xDB

#define SSD1306_PAGE_HEIGHT         8

#define SSD1306_NUM_PAGES           SSD1306_HEIGHT / SSD1306_PAGE_HEIGHT
#define SSD1306_BUF_LEN             SSD1306_NUM_PAGES * SSD1306_WIDTH


#define SSD1306_WRITE_MODE         0xFE
#define SSD1306_READ_MODE          0xFF

//struct used to determine the area to be rendered
struct render_area {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
};


void calc_render_area_buflen(struct render_area *area);
void SSD1306_send_cmd(uint8_t cmd);
void SSD1306_send_cmd_list(uint8_t *buf, int num);
void SSD1306_send_buf(uint8_t buf[], int buflen);
void SSD1306_init();
void render(uint8_t *buf, struct render_area *area);

#endif 
