#include "hwlib.hpp"
#include "snake.hpp"
#include "matrix.hpp"

bool snake::get_pin(hwlib::target::pin_in& button){
    button.refresh();
    return button.read();
}

void snake::play(){   
    auto din = hwlib::target::pin_out( hwlib::target::pins::d8 );
    auto cs = hwlib::target::pin_out( hwlib::target::pins::d9 );
    auto clk = hwlib::target::pin_out( hwlib::target::pins::d10 );
    matrix leds(din, cs, clk, 6);
    leds.set_up();

    int update_speed = 750;

    bool start_screen[8][8] = {
                            {1,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0}
                            };
    leds.send_array(start_screen, 1);
    int snakecords[2] = {0,0}; //y, x        always start the game on (0,0). int snakecords[3] = {1,0,0} screen, y, x


    bool new_screen[8][8] = {
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0}
                        };


    bool sw1 = 0;   //left
    bool sw2 = 0;   //up
    bool sw3 = 0;   //down
    bool sw4 = 1;   //right     always start the game by going right
    for(;;){
        //check witch button is pressed
        for(int i = 0; i < update_speed; i++){
            if(get_pin(swleft) ){
                sw1 = true;
                sw2 = 0;
                sw3 = 0;
                sw4 = 0;
            }
            if(get_pin(swup) ){
                sw2 = true;
                sw1 = 0;
                sw3 = 0;
                sw4 = 0;
            }
            if(get_pin(swdown) ){
                sw3 = true;
                sw1 = 0;
                sw2 = 0;
                sw4 = 0;
            }
            if(get_pin(swright) ){
                sw4 = true;
                sw1 = 0;
                sw2 = 0;
                sw3 = 0;
            }
            hwlib::wait_ms(1);
        }
        
        //update new screen with snakecords
        new_screen[ snakecords[0] ] [ snakecords[1] ] = 0;
        if(sw1){//left
            hwlib::cout<<"1\n";
            snakecords[1] = (snakecords[1] -1);
        }
        if(sw2){//up
            hwlib::cout<<"2\n";
            snakecords[0] = (snakecords[0] -1);
        }
        if(sw3){//down
            hwlib::cout<<"3\n";
            snakecords[0] = (snakecords[0] +1);
        }
        if(sw4){//right
            hwlib::cout<<"4\n";
            snakecords[1] = (snakecords[1] +1);
        }

        //game over if you touch the walls
        if(snakecords[0] > 7){
            snakecords[0] = 7;
        }
        if(snakecords[1] > 7){
            snakecords[1] = 7;
        }
        if(snakecords[0] < 0){
            snakecords[0] = 0;
        }
        if(snakecords[1] < 0){
            snakecords[1] = 0;
        }

        new_screen[ snakecords[0] ] [ snakecords[1] ] = 1;
        leds.send_array(new_screen, 1);
    }
}

void snake::game_over