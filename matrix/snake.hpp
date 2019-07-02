#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "matrix.hpp"
#include "hwlib.hpp"

class snake{
private:
    hwlib::target::pin_in swleft;
    hwlib::target::pin_in swup;
    hwlib::target::pin_in swdown;
    hwlib::target::pin_in swright;

    matrix leds;

    bool empty_screen[8][8] = {
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
    
    bool screens[6][8][8] = {
                            {empty_screen},
                            {empty_screen},
                            {empty_screen},
                            {empty_screen},
                            {empty_screen},
                            {empty_screen}
                            };//screen, y, x

    bool sw1;   //left
    bool sw2;   //up
    bool sw3;   //down
    bool sw4;   //right     always start the game by going right


    int update_speed = 250;
    int way; //witch way the snke is going

    int snakecords[3]; //y, x, screen, always start the game on (0,0) on screen 0.
    int applecords[3] = {0,0,0};

    int points = 0;
    int tail[ (8*8*6) ][3];
    int tail_way[ (8*8*6) ][1];
    // for(int i = 0; i < (8*8*6); i++){
    //     tail[i] = 0;
    //     tail_way[i] = 0;
    //     tail_way[i][0] = 0;
    //     for(int j = 0; j < 3; j++){
    //         tail[i][j] = 0
    //     }
    // }
    

public:
    snake(  hwlib::target::pin_in swleft,
            hwlib::target::pin_in swup,
            hwlib::target::pin_in swdown,
            hwlib::target::pin_in swright,
            matrix& leds):
            swleft(swleft), swup(swup), swdown(swdown), swright(swright), leds(leds)
            {
                snakecords[0] = 0;
                snakecords[1] = 0;
                snakecords[2] = 0;
                
                for(int i = 0; i <= 5; i++){
                    int temp[3] = {0,0,i};
                    set_screen(temp, 0);
                }
               
                //clear screen 5
                // for(int y = 0; y < 8; y++){
                //     for(int x = 0; x < 8; x++){
                //         int temp[3] = {y,x,6};
                //         set_screen(temp, 0);
                //     }
                // }

                set_screen(snakecords, 1); //start position
                apple();                   //first apple
                sw1 = 0;
                sw2 = 0;
                sw3 = 0;
                sw4 = 1;
            }

    bool get_pin(hwlib::target::pin_in& button);

    void set_screen(int cords[3], bool state);

    void write_screen(matrix& leds);

    void update_cords(int cords[3], int temp_way);

    void next_screen(int snakecords[3]);

    void apple();

    void play();

    void game_over(matrix& leds);
};

#endif