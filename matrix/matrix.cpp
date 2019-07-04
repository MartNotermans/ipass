//          Copyright Mart Notermans 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "hwlib.hpp"
#include "matrix.hpp"

void matrix::set_up(){
    for(int i = 0; i <= n_screens; i++){
        int scanlimit8 = 0xB07;
        int decodemode = 0x900;
        int low  = 0xA00;
        //int high = 0xA0F;
        int shutdown_off = 0xCF1;

        send_data(scanlimit8, i);
        send_data(decodemode, i);
        send_data(low, i);
        send_data(shutdown_off, i);
        clear(i);
    }
}

void matrix::pulse_clk(){
    hwlib::wait_us(1);
    clk.write(1);
    hwlib::wait_us(3);
    clk.write(0);
    hwlib::wait_us(2);
}

void matrix::send_data(const int data, int screen){
    uint16_t data_b = data;
    cs.write(0);


        no_op(n_screens-screen);

    for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
        din.write( (data_b & (1<<bit_cnt) ) !=0 );
        //hwlib::cout<<( (data_b & (1<<bit_cnt) ) !=0);
        pulse_clk();
    }

        no_op(screen-1);

    //hwlib::cout<<"\n";
    cs.write(1);
}

void matrix::send_array(bool pixels[8][8], int screen){
    for(int i = 0; i < 8; i++){
        uint8_t data = 0;
        for(int j = 0; j < 8; j++){
            data = data | ( pixels[j][i]<< j );
            //hwlib::cout<<( (data & (1<<j) ) !=0);
        }
        //hwlib::cout<<"\n";
        uint8_t row = i+1;
        uint16_t row_data = ( (row<<8) | data);
        send_data(row_data, screen);
    }
}

void matrix::no_op(int times){
    for(int i = 0; i < (times*16); i++){
        din.write(0);
        pulse_clk();
    }
}

void matrix::clear(int screen){
    bool empty[8][8] = {
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0}
                        };
    send_array(empty, screen);
}

void matrix::flash(int times, int speed){
    int low  = 0xA00;
    int high = 0xA0F;
    for(int i = 0; i< times; i++){
        for(int j = 1; j <= n_screens; j++){
            send_data(high, j);
        }
        hwlib::wait_ms(speed);
        for(int j = 1; j <= n_screens; j++){
            send_data(low, j);
        }
        hwlib::wait_ms(speed);
    }
}