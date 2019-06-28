#include <hwlib.hpp>
//Din   pin 8
//cs    pin 9
//clk   pin 10

class matrix{
public:
    hwlib::target::pin_out din;
    hwlib::target::pin_out cs;
    hwlib::target::pin_out clk;
    int n_screens;

    int low  = 0b0000101000000000;
    int high = 0b0000101000001111;
    
    matrix(hwlib::target::pin_out &din, hwlib::target::pin_out &cs, hwlib::target::pin_out &clk, int n_screens = 1):
    din (din), cs (cs), clk (clk), n_screens (n_screens)
    {
        hwlib::cout<<"matrix made\n";
    }

//sending data ---------------------------------------------------------------------------------------------    
    void pulse_clk(){
        hwlib::wait_us(1);
        clk.write(1);
        hwlib::wait_us(3);
        clk.write(0);
        hwlib::wait_us(2);
    }

    //send 16 bits of data
    //void send_data(const int data, int screen = 0){
    void send_data(const int data){
        uint16_t data_b = data;
        cs.write(0);

        for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
            din.write( (data_b & (1<<bit_cnt) ) !=0 );
//            hwlib::cout<<( (data_b & (1<<bit_cnt) ) !=0);
            pulse_clk();
        }

//        hwlib::cout<<"\n";
        cs.write(1);
    }

    //send an array of 8 rows of data to 1 screen
    void send_rows(int pixels[8]){
        for(int i = 0; i < 8; i++){
            uint8_t row = i+1;
            uint16_t data = ( (row<<8) | pixels[i]);
            send_data(data);
        }
    }

    //send a dubbel array of boolians to 1 screen
    void send_array(bool pixels[8][8]){
        for(int i = 0; i < 8; i++){
            uint8_t data = 0;
            for(int j = 0; j < 8; j++){
                data = data | ( pixels[j][i]<< j );
//                hwlib::cout<<( (data & (1<<j) ) !=0);
            }
//            hwlib::cout<<"\n";
            uint8_t row = i+1;
            uint16_t row_data = ( (row<<8) | data);
            send_data(row_data);
        }
    }

    //moves a matrix on the screen from the left to the middle
    void move(bool pixels[8][8], int delay = 50){
        for(int i = 8; i > 0; i--){
            bool temp[8][8];
            for(int y = 0; y < 8; y++){
                for(int x = 0; x < 8; x++){
                    if( (x+i) > 8){
                        temp[y][x] = 0;
                    }else{
                        temp[y][x] = pixels[y][x+i-1];
                    }
//                    hwlib::cout<<temp[y][x];
                }
//                hwlib::cout<<"\n";
            }
            send_array(temp);
//            hwlib::cout<<"--------\n";
            hwlib::wait_ms(delay);
        }
    }

    // void move2(bool pixels[8][8]){
    //     for(int i = 8; i > -8; i--){
    //         bool temp[8][8];
    //         for(int y = 0; y < 8; y++){
    //             for(int x = 0; x < 8; x++){
    //                 if( (x+i) > 8){
    //                     temp[y][x] = 0;
    //                 }else{
    //                     temp[y][x] = pixels[y][x+i-1];
    //                 }
    //                 hwlib::cout<<temp[y][x];
    //             }
    //             hwlib::cout<<"\n";
    //         }
    //         send_array(temp);
    //         hwlib::cout<<i<<"  -----\n";
    //         hwlib::wait_ms(20);
    //     }
    // }

    // send 16 zero's as no op
    void no_op(int times = 1){
        for(int i = 0; i < times*8; i++){
            din.write(0);
            pulse_clk();
        }
    }

    //send an empty screen via write()
    void clear(){
        hwlib::cout<<"clear\n";
        int zero [8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        send_rows(zero);
    }
//sending data ---------------------------------------------------------------------------------------------    
    

//setup ----------------------------------------------------------------------------------------------------    
    // void set_up(const int data){
    //     uint16_t data_b = data;
    //     for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
    //         din.write( (data_b & (1<<bit_cnt) ) !=0 );
    //         //hwlib::cout<<( (data_b & (1<<bit_cnt) ) !=0);
    //         pulse_clk();
    //     }
    // }
    
    void shutdown_off(){
        hwlib::cout<<"shutdown_off\n";
        int shutdown_off = 0b0000110011110001;
                           //1234567812345678
        send_data(shutdown_off);
    }

    void scanlimit8(){
        hwlib::cout<<"scanlimit8\n";
        int scanlimit8 = 0b0000101100000111;
                         //1234567812345678
        send_data(scanlimit8);
    }

    void decodemode(){
        hwlib::cout<<"decodemode\n";
        int decodemode = 0b0000100100000000;
                         //1234567812345678
        send_data(decodemode);
    }
//setup ----------------------------------------------------------------------------------------------------    

//brightness -----------------------------------------------------------------------------------------------    
    void intensity_low(){
        hwlib::cout<<"intensity low\n";
        send_data(low);
    }

    void intensity_high(){
        hwlib::cout<<"intensity high\n";
        send_data(high);
    }

    void flash(int times, int speed = 150){
        for(int i = 0; i< times; i++){
            send_data(high);
            hwlib::wait_ms(speed);
            send_data(low);
            hwlib::wait_ms(speed);
        }
    }

    // void pulse(int times, int speed = 40){
    //     for(int i = 0; i< times; i++){
    //         for(int j = 0; j < 16; j++){
    //             uint8_t data = j;
    //             send_data(0xa<<8 | data);
    //             hwlib::cout<<data<<" j\n";
    //             hwlib::wait_ms(speed);
    //         }

    //         //hwlib::wait_ms(500);

    //         for(int k = 15; k >= 0; k--){
    //             uint8_t data = k;
    //             send_data(0xa<<8 | data);
    //             hwlib::cout<<data<<" k\n";
    //             hwlib::wait_ms(speed);
    //         }
    //         hwlib::cout<<i<<" i------\n";
    //     }
    // }
//brightness -----------------------------------------------------------------------------------------------    

    void snake(bool dot[8][8], int movement[16], int speed = 500){
        send_array(dot);
        hwlib::wait_ms(speed);
        for(int i = 0; i < 16; i++){                        //loop door movement
            if( (movement[i] == 1) | (movement[i] == 4) ){  //1 en 4 of 2 en 3
//                hwlib::cout<<"1 of 4\n";
                for(int y = 0; y < 8; y++){                 //loop door de rijen
                    for(int x = 0; x < 8; x++){             //loop door 1 rij
                        if(movement[i] == 1){
                            //hwlib::cout<<"1\n";
                            dot[y][x] = dot[y+1][x];
                        }else if(movement[i] == 4){
                            //hwlib::cout<<"4\n";
                            dot[y][x] = dot[y][x+1];
                        }
                    }
                }

            }else{
//                hwlib::cout<<"2 of 3\n";
                for(int y = 8; y > 0; y--){                 //loop door de rijen
                    for(int x = 8; x > 0; x--){             //loop door 1 rij
                        if(movement[i] == 2){
                            //hwlib::cout<<x<<">"<<x-1<<"\n";
                            dot[y][x] = dot[y][x-1];
                        }else if(movement[i] == 3){
                            //hwlib::cout<<"3\n";
                            dot[y][x] = dot[y-1][x];
                        }
                    }
                }
            }


            for(int j = 0; j < 8; j++){                     //nieuwe rijen leeg maken
                if(movement[i] == 1){
                    dot[7][j] = 0;
                }else if(movement[i] == 2){
                    dot[j][0] = 0;
                }else if(movement[i] == 3){
                    dot[0][j] = 0;
                }else if(movement[i] == 4){
                    dot[j][7] = 0;
                }
            }

            send_array(dot);
            //hwlib::cout<<movement[i]<<"---\n";
            hwlib::wait_ms(speed);
        }
    }

};

int main(){
    hwlib::wait_ms(500);
   
    auto din = hwlib::target::pin_out( hwlib::target::pins::d8 );
    auto cs = hwlib::target::pin_out( hwlib::target::pins::d9 );
    auto clk = hwlib::target::pin_out( hwlib::target::pins::d10 );
    
    matrix leds(din, cs, clk, 1);

    leds.scanlimit8();
    leds.decodemode();
    leds.intensity_low();
    leds.shutdown_off();
    leds.clear();
    hwlib::cout<<"----------------\n";//---------------------------------------------------------------------------------
    
    hwlib::wait_ms(1000);

    //int letter_A [8] = {0x00, 0x7C, 0x7E, 0x13, 0x13, 0x7E, 0x7C, 0x00};
    // int letter_B [8] = {0x41, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00};
    // int letter_S [8] = {0x26, 0x6F, 0x4D, 0x59, 0x73, 0x32, 0x00, 0x00};

    //int green_lantern [8] = { 0x00, 0x99, 0xBD, 0xE7, 0xE7, 0xBD, 0x99, 0x00 };

    // bool array_A[8][8] = {
    //                         {0,0,0,0,0,0,0,0},
    //                         {0,0,0,1,1,0,0,0},
    //                         {0,0,1,1,1,1,0,0},
    //                         {0,1,1,0,0,1,1,0},
    //                         {0,1,1,0,0,1,1,0},
    //                         {0,1,1,1,1,1,1,0},
    //                         {0,1,1,0,0,1,1,0},
    //                         {0,1,1,0,0,1,1,0}
    //                         };

    // leds.send_array(array_A);


    bool dot[8][8] = {
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,1,1,0,0,0},
                            {0,0,0,1,1,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0}
                            };

    //north | up    = 1
    //east  | right = 2
    //south | down  = 3
    //west  | left  = 4
    int movement[16] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
    //int movement2[4] = {1,2,3,4};
    //leds.move(dot);
    leds.snake(dot, movement);






    // leds.send_rows(letter_B);
    // hwlib::wait_ms(1000);
    // leds.send_rows(letter_A);
    // hwlib::wait_ms(1000);
    // leds.send_rows(letter_S);

    //leds.move(letter_A);
    //leds.move(array_A, 20);

    // int testing = 0b0000000101011101;
    // leds.send_data(testing);



    //leds.write(letter_A, 2);





    //leds.flash(3);

    hwlib::cout<<"done\n";
}