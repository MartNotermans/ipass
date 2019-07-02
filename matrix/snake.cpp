#include "hwlib.hpp"
#include "snake.hpp"
//#include "matrix.hpp"

bool snake::get_pin(hwlib::target::pin_in& button){
    button.refresh();
    return button.read();
}

void snake::set_screen(int cords[3], bool state){
    screens[ cords[2] ][cords [0] ][cords[1] ] = state;
}

void snake::write_screen(matrix& leds){
    for(int i = 0; i <= 5; i++){
        leds.send_array(screens[i], i+1);
    }
}

void snake::update_cords(int cords[3], int temp_way){
    if(temp_way == 1){//left
        //hwlib::cout<<"1\n";
        cords[1] = (cords[1] -1);
    }
    if(temp_way == 2){//up
        //hwlib::cout<<"2\n";
        cords[0] = (cords[0] -1);
    }
    if(temp_way == 3){//down
        //hwlib::cout<<"3\n";
        cords[0] = (cords[0] +1);
    }
    if(temp_way == 4){//right
        //hwlib::cout<<"4\n";
        cords[1] = (cords[1] +1);
    }
}

void snake::next_screen(int cords[3]){
    if(cords[1] > 7){//right
//        int temp_cords[3] = {cords[0],7,cords[2]};
//        set_screen(temp_cords , 0); //set old position to 0
        cords[1] = 0; //go to x 0 on new screen
        cords[2] = cords[2] + 1; //next screen
    }
    if(cords[1] < 0){//left
//        int temp_cords[3] = {cords[0],0,cords[2]};
//        set_screen(temp_cords, 0);
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

        if( (snakecords[0] == applecords[0]) && (snakecords[1] == applecords[1]) && (snakecords[2] == applecords[2]) ){
            points++;
            hwlib::cout<<"points "<<points<<"\n";
            tail[points][0] = applecords[0];
            tail[points][1] = applecords[1];
            tail[points][2] = applecords[2];
            tail_way[points][0] = way;

            apple();
            
            if(update_speed != 20){ //update speed
                update_speed = update_speed - 10;
            }
        }

        set_screen(snakecords, 0); //set old position to 0

        update_cords(snakecords, way); //update snake position

        //move to next screen?
        next_screen(snakecords);

        

        //update tail
        for(int i = 1; i <= points; i++){
            if(way != tail_way[i][0]){
                tail_way[i][0] = way;
            }

            int cords_tail_end[3] = { tail[points][0], tail[points][1], tail[points][2]};
            set_screen(cords_tail_end, 0);
            
            set_screen(tail[i], 1);            
            update_cords(tail[i], tail_way[i][0]);
            next_screen(tail[i]);   
        }

        set_screen(snakecords, 1); //set new position to 1

        //game over if you touch the walls
        if(snakecords[0] > 7){
            game_over(leds);
            break;
        }
        if(snakecords[2] > 5){
            game_over(leds);
            break;
        }
        if(snakecords[0] < 0){
            game_over(leds);
            break;
        }
        if(snakecords[2] < 0){
            game_over(leds);
            break;
        }

        write_screen(leds);
    }
}

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

    for(int i = 0; i < 3; i++){
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
    for(int i = 0; i <= 5; i++){
        leds.clear(i);
    }
}