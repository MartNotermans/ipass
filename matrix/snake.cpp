#include "hwlib.hpp"
#include "snake.hpp"
//#include "matrix.hpp"

bool snake::get_pin(hwlib::target::pin_in& button){
    button.refresh();
    return button.read();
}

void snake::play(matrix& leds){
    int update_speed = 250;
    bool easy = false;

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
    
    bool start_screen[8][8] = {empty_screen};
    start_screen[0][0] = 1;
    leds.send_array(start_screen, 1);

    bool new_screen[8][8] = {empty_screen};

    int snakecords[3] = {0,0,1}; //y, x, screen, always start the game on (0,0) on screen 1.



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
        
        new_screen[ snakecords[0] ] [ snakecords[1] ] = 0; //set old position to 0
        //update snakecords
        if(sw1){//left
            //hwlib::cout<<"1\n";
            snakecords[1] = (snakecords[1] -1);
        }
        if(sw2){//up
            //hwlib::cout<<"2\n";
            snakecords[0] = (snakecords[0] -1);
        }
        if(sw3){//down
            //hwlib::cout<<"3\n";
            snakecords[0] = (snakecords[0] +1);
        }
        if(sw4){//right
            //hwlib::cout<<"4\n";
            snakecords[1] = (snakecords[1] +1);
        }

        //hwlib::cout<<"y"<<snakecords[0]<<"x"<<snakecords[1]<<"s"<<snakecords[2]<<"-\n";

        //move to next screen
        if(snakecords[1] > 7){//right
            bool old_screen[8][8] = {new_screen};
            old_screen[ snakecords[0] ] [ 7 ] = 0;
            old_screen[0][0] = 0;
            leds.send_array(old_screen, snakecords[2]);

            snakecords[1] = 0; //go to pos 0 on new screen
            snakecords[2] = snakecords[2] + 1; //next screen
        }
        if(snakecords[1] < 0){//left
            bool old_screen[8][8] = {new_screen};
            old_screen[ snakecords[0] ] [ 0 ] = 0;
            old_screen[0][0] = 0;
            leds.send_array(old_screen, snakecords[2]);

            //new_screen[ snakecords[0] ] [ 0 ] = 0;
            snakecords[1] = 7; //go to pos 0 on new screen
            snakecords[2] = snakecords[2] - 1; //next screen
        }

        //hwlib::cout<<"y"<<snakecords[0]<<"x"<<snakecords[1]<<"s"<<snakecords[2]<<"\n";

        if(easy == 1){
            //if you touch the walls you stand still
            if(snakecords[0] > 7){//up
                snakecords[0] = 7;
            }
            if(snakecords[1] > 7 && snakecords[2] == 6){//right
                snakecords[1] = 7;
            }
            if(snakecords[0] < 0){//down
                snakecords[0] = 0;
            }
            if(snakecords[1] < 0 && snakecords[2] == 1){//left
                snakecords[1] = 0;
            }
        }else{
            //game over if you touch the walls
            if(snakecords[0] > 7){
                game_over(leds);
                break;
            }
            if(snakecords[2] > 6){
                game_over(leds);
                break;
            }
            if(snakecords[0] < 0){
                game_over(leds);
                break;
            }
            if(snakecords[2] < 1){
                game_over(leds);
                break;
            }
        }

        new_screen[ snakecords[0] ] [ snakecords[1] ] = 1; //set new position to 1
        leds.send_array(new_screen, snakecords[2]);
    }
}

void snake::game_over(matrix& leds){
    bool letter_G[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,1,1,1,0,0},
                            {0,0,1,1,1,1,1,0},
                            {0,1,1,0,0,0,0,0},
                            {0,1,1,0,1,1,1,0},
                            {0,1,1,0,0,0,1,0},
                            {0,0,1,1,0,0,1,0},
                            {0,0,0,1,1,1,0,0}
                            };

    bool letter_A[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,1,1,0,0,0},
                            {0,0,1,1,1,1,0,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,1,1,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0}
                            };

    bool letter_M[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,1,1,1,1,0},
                            {0,1,1,1,1,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0}
                            };

    bool letter_E[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,1,1,1,1,1,1,0},
                            {0,1,1,1,1,1,0,0},
                            {0,1,1,0,0,0,0,0},
                            {0,1,1,1,1,1,0,0},
                            {0,1,1,0,0,0,0,0},
                            {0,1,1,1,1,1,0,0},
                            {0,1,1,1,1,1,1,0}
                            };

    bool letter_O[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,0,1,1,1,1,0,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,0,1,1,1,1,0,0}
                            };

    bool letter_V[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,0,1,1,1,1,0,0},
                            {0,0,0,1,1,0,0,0}
                            };

    bool letter_R[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,1,1,1,1,1,0,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,1,1,1,0,0},
                            {0,1,1,1,1,0,0,0},
                            {0,1,1,0,1,1,0,0},
                            {0,1,1,0,0,1,1,0}
                            };
    
    leds.clear(1);
    leds.clear(6);

    for(;;){
        leds.send_array(letter_G, 2);
        leds.send_array(letter_A, 3);
        leds.send_array(letter_M, 4);
        leds.send_array(letter_E, 5);
        hwlib::wait_ms(1000);
        leds.send_array(letter_O, 2);
        leds.send_array(letter_V, 3);
        leds.send_array(letter_E, 4);
        leds.send_array(letter_R, 5);
        hwlib::wait_ms(1000);
    }
}