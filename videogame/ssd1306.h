#ifndef SSD1306_H  
#define SSD1306_H  

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
