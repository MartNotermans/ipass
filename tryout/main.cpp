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
    
    matrix(hwlib::target::pin_out &din, hwlib::target::pin_out &cs, hwlib::target::pin_out &clk, int n_screens = 6):
    din (din), cs (cs), clk (clk), n_screens (n_screens)
    {
        clk.write(0);
        cs.write(1);
        if(n_screens > 1){
            hwlib::cout<<"matrix made with "<<n_screens<<" screens\n";
        }else{
            hwlib::cout<<"matrix made with 1 screen\n";
        }
        hwlib::wait_ms(250);
    }

//sending data ---------------------------------------------------------------------------------------------    
    void pulse_clk(){
        hwlib::wait_us(1);
        clk.write(1);
        hwlib::wait_us(3);
        clk.write(0);
        hwlib::wait_us(2);
    }

    //send 16 bits of data to 1 screen
    void send_data(const int data, int screen = 1){
        uint16_t data_b = data;
        cs.write(0);

        no_op(n_screens-screen);

        for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
            din.write( (data_b & (1<<bit_cnt) ) !=0 );
//            hwlib::cout<<( (data_b & (1<<bit_cnt) ) !=0);
            pulse_clk();
        }

        no_op(screen-1);
//        hwlib::cout<<"\n";
        cs.write(1);
    }

    //send an array of 8 rows of data to 1 screen
    // void send_rows(int pixels[8]){
    //     for(int i = 0; i < 8; i++){
    //         uint8_t row = i+1;
    //         uint16_t data = ( (row<<8) | pixels[i]);
    //         send_data(data);
    //     }
    // }

    //send a dubbel array of boolians to 1 screen
    void send_array(bool pixels[8][8], int screen){
        for(int i = 0; i < 8; i++){
            uint8_t data = 0;
            for(int j = 0; j < 8; j++){
                data = data | ( pixels[j][i]<< j );
//                hwlib::cout<<( (data & (1<<j) ) !=0);
            }
//            hwlib::cout<<"\n";
            uint8_t row = i+1;
            uint16_t row_data = ( (row<<8) | data);
            send_data(row_data, screen);
        }
    }

//     //moves a matrix on the screen from the left to the middle
//     void move(bool pixels[8][8], int delay = 50, int screen){
//         for(int i = 8; i > 0; i--){
//             bool temp[8][8];
//             for(int y = 0; y < 8; y++){
//                 for(int x = 0; x < 8; x++){
//                     if( (x+i) > 8){
//                         temp[y][x] = 0;
//                     }else{
//                         temp[y][x] = pixels[y][x+i-1];
//                     }
// //                    hwlib::cout<<temp[y][x];
//                 }
// //                hwlib::cout<<"\n";
//             }
//             send_array(temp, screen);
// //            hwlib::cout<<"--------\n";
//             hwlib::wait_ms(delay);
//         }
//     }

    //send 16 zero's as no op
    void no_op(int times = 1){
        for(int i = 0; i < (times*16); i++){
            din.write(0);
            pulse_clk();
        }
    }

    //send an empty screen via write()
    void clear(int screen){
        //hwlib::cout<<"clear "<<screen<<"\n";
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
//sending data ---------------------------------------------------------------------------------------------    
    

//setup ----------------------------------------------------------------------------------------------------    
    void set_up(){//data moet in hex
        for(int i = 1; i <= n_screens; i++){
            int scanlimit8 = 0b0000101100000111;
            send_data(scanlimit8, i);

            int decodemode = 0b0000100100000000;
            send_data(decodemode);

            int low  = 0b0000101000000000;
            //int high = 0b0000101000001111;
            send_data(low, i);

            int shutdown_off = 0b0000110011110001;
            send_data(shutdown_off);

            clear(i);
        }
    }
//setup ----------------------------------------------------------------------------------------------------    

//brightness -----------------------------------------------------------------------------------------------    
    // void flash(int times, int speed = 150){
    //     for(int i = 0; i< times; i++){
    //         send_data(high);
    //         hwlib::wait_ms(speed);
    //         send_data(low);
    //         hwlib::wait_ms(speed);
    //     }
    // }

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

    void snake(bool dot[8][8], int movement[4], int speed = 1000, int screen = 1){
        send_array(dot, screen);
        hwlib::wait_ms(speed);
        for(int i = 0; i < 4; i++){                        //loop door movement
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

            send_array(dot, screen);
            //hwlib::cout<<movement[i]<<"---\n";
            hwlib::wait_ms(speed);
        }
    }

};

int main(){
    //Din   pin 8
    //cs    pin 9
    //clk   pin 10
    hwlib::wait_ms(500);
   
    auto din = hwlib::target::pin_out( hwlib::target::pins::d8 );
    auto cs = hwlib::target::pin_out( hwlib::target::pins::d9 );
    auto clk = hwlib::target::pin_out( hwlib::target::pins::d10 );
    
    matrix leds(din, cs, clk, 6);
    leds.set_up();
    //hwlib::cout<<"----------------\n";




    //int letter_A [8] = {0x00, 0x7C, 0x7E, 0x13, 0x13, 0x7E, 0x7C, 0x00};
    // int letter_B [8] = {0x41, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00};
    // int letter_S [8] = {0x26, 0x6F, 0x4D, 0x59, 0x73, 0x32, 0x00, 0x00};
    //int green_lantern [8] = { 0x00, 0x99, 0xBD, 0xE7, 0xE7, 0xBD, 0x99, 0x00 };
    //send_rows


    bool array_A[8][8] = {
                            {1,1,0,0,0,0,1,1},
                            {1,0,0,1,1,0,0,1},
                            {0,0,1,1,1,1,0,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,1,1,1,1,0},
                            {0,1,1,0,0,1,1,0},
                            {0,1,1,0,0,1,1,0}
                            };

    leds.send_array(array_A, 1);
    //leds.send_array(array_A, 2);

    bool dot[8][8] = {
                            {1,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,1,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0}
                            };
    // leds.send_array(dot, 3);

    bool thing[8][8] = {
                            {1,1,1,1,1,1,1,1},
                            {1,1,0,0,0,0,1,1},
                            {1,0,1,0,0,1,0,1},
                            {1,0,0,1,1,0,0,1},
                            {1,0,0,1,1,0,0,1},
                            {1,0,1,0,0,1,0,1},
                            {1,1,0,0,0,0,1,1},
                            {1,1,1,1,1,1,1,1}
                            };
    leds.send_array(thing, 4);
    leds.send_array(thing, 6);

    // hwlib::wait_ms(500);

    //north | up    = 1
    //east  | right = 2
    //south | down  = 3
    //west  | left  = 4
    // int movement[16] = {2,3,2,2,2,2,3,3,4,4,3,2,1,1,2,3};
    int movement[4] = {2,3,4,1};
    leds.snake(dot, movement, 500, 3);

    // leds.send_array(thing, 6);
    // leds.send_array(dot, 5);
    // leds.send_array(dot, 1);

    hwlib::cout<<"done\n";
}