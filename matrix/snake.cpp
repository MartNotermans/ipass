//          Copyright Mart Notermans 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



#include "hwlib.hpp"
#include "snake.hpp"

bool snake::get_pin(hwlib::target::pin_in& button){
    button.refresh();
    return button.read();
}

void snake::button(){
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
    
    if(sw1){
        way = 1;
    }
    if(sw2){
        way = 2;
    }
    if(sw3){
        way = 3;
    }
    if(sw4){
        way = 4;
    }
}

bool snake::equals(int lhs[3], int rhs[3]){
    return( (lhs[0] == rhs[0]) &&
            (lhs[1] == rhs[1]) &&
            (lhs[2] == rhs[2]) );
}

void snake::set_screen(int cords[3], bool state){
    screens[ cords[2] ][cords[0] ][cords[1] ] = state;
}

void snake::write_screen(matrix& leds){
    for(int i = 0; i <= 5; i++){
        leds.send_array(screens[i], i+1);
    }
}

void snake::update_cords(int cords[3], int temp_way){
    if(temp_way == 1){//left
        cords[1] = (cords[1] -1);
    }
    if(temp_way == 2){//up
        cords[0] = (cords[0] -1);
    }
    if(temp_way == 3){//down
        cords[0] = (cords[0] +1);
    }
    if(temp_way == 4){//right
        cords[1] = (cords[1] +1);
    }
}

void snake::next_screen(int cords[3]){
    if(cords[1] > 7){//right
        cords[1] = 0; //go to x 0 on new screen
        cords[2] = cords[2] + 1; //next screen
    }
    if(cords[1] < 0){//left
        cords[1] = 7; //go to x 7 on new screen
        cords[2] = cords[2] - 1; //next screen
    }
}

void snake::apple(){
    srand(hwlib::now_us() );
    applecords[0] = rand() % 8;
    applecords[1] = rand() % 8;
    applecords[2] = rand() % 6;
    set_screen(applecords, 1);
}

void snake::play(){
    while(playing){
        button();

        for(int i = 0; i <= points; i++){
            set_screen(tail[i], 0);
        }

        if( equals(tail[0], applecords) ){
            points++;
            apple();
            if(update_speed > 20){ //update speed
                update_speed = update_speed - 10;
            }
        }

        for(int i = points; i >= 1; i--){
            tail[i][0] = tail[i-1][0];
            tail[i][1] = tail[i-1][1];
            tail[i][2] = tail[i-1][2];
        }

        update_cords(tail[0], way);
        next_screen(tail[0]);

        for(int i = 0; i <= points; i++){
            set_screen(tail[i], 1);
        }

        //game over if you touch the walls
        if(tail[0][0] > 7){
            game_over(leds);
            break;
        }
        if(tail[0][2] > 5){
            game_over(leds);
            break;
        }
        if(tail[0][0] < 0){
            game_over(leds);
            break;
        }
        if(tail[0][2] < 0){
            game_over(leds);
            break;
        }

        for(int i = 1; i <= points; i++){
            if( equals(tail[0], tail[i]) ){
                game_over(leds);
                break;
            }
        }


        if(playing){
            write_screen(leds);
        }

    } //endless loop
} //class

void snake::game_over(matrix& leds){
    bool letter_G[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,0,1,1,1,1,0,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,0,0,0},
                            {0,1,1,0,1,1,1,0},
                            {0,1,1,0,0,0,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,0,1,1,1,1,0,0}
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
    
    for(int i = 1; i <= 6; i++){
        leds.clear(i);
        leds.send_data(0xA0F, i);
    }

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

    playing = 0;
    for(int i = 0; i <= 5; i++){
        leds.clear(i);
    }
}