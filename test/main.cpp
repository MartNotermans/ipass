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
    
    matrix(hwlib::target::pin_out din, hwlib::target::pin_out cs, hwlib::target::pin_out clk, int n_screens = 1):
    din (din), cs (cs), clk (clk), n_screens (n_screens)
    {
        if(n_screens > 1){
            hwlib::cout<<"matrix made with "<<n_screens<<" screens\n";
        }else{
            hwlib::cout<<"matrix made with 1 screen\n";
        }
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
    void send_data(const int data, int screen = 0){
        uint16_t data_b = data;
        cs.write(0);
        hwlib::cout<<"cs 0\n";

        if(screen != 0){
            hwlib::cout<<"before data "<< (screen-1) <<" \n";
            for(int i = 1; i < (screen-1); i++){
                no_op(2);
            }
        }

        for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
            din.write( (data_b & (1<<bit_cnt) ) !=0 );
            hwlib::cout<<( (data_b & (1<<bit_cnt) ) !=0);
            pulse_clk();
        }

        if(screen != 0){
            hwlib::cout<<"after data "<< (n_screens - screen) <<" \n";
            for(int i = 1; i < (n_screens - screen); i++){
                no_op(2);
            }
        }

        //hwlib::cout<<"\n";
        cs.write(1);
        hwlib::cout<<"cs 1\n\n";
    }

    //send an array of 8 rows of data to 1 screen
    void write(int pixels[8], int screen = 0){
        for(int i = 0; i < 8; i++){
            uint8_t row = i+1;
            uint16_t data = ( (row<<8) | pixels[i]);
            send_data(data, screen);
        }
    }

    // send 16 zero's as no op
    void no_op(int times = 1){
        for(int i = 0; i < times*8; i++){
            din.write(0);
        }
    }

    //send an empty screen via write()
    void clear(int screen = 0){
        hwlib::cout<<"clear\n";
        int zero [8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        write(zero, screen);
    }
//sending data ---------------------------------------------------------------------------------------------    
    

//setup ----------------------------------------------------------------------------------------------------    
    void set_up(const int data){
        uint16_t data_b = data;
        for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
            din.write( (data_b & (1<<bit_cnt) ) !=0 );
            //hwlib::cout<<( (data_b & (1<<bit_cnt) ) !=0);
            pulse_clk();
        }
    }
    
    void shutdown_off(){
        hwlib::cout<<"shutdown_off\n";
        int shutdown_off = 0b0000110011110001;
                           //1234567812345678
        set_up(shutdown_off);
    }

    void scanlimit8(){
        hwlib::cout<<"scanlimit8\n";
        int scanlimit8 = 0b0000101100000111;
                         //1234567812345678
        set_up(scanlimit8);
    }

    void decodemode(){
        hwlib::cout<<"decodemode\n";
        int decodemode = 0b0000100100000000;
                         //1234567812345678
        set_up(decodemode);
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

    void pulse(int times, int speed = 40){
        for(int i = 0; i< times; i++){
            for(int j = 0; j < 16; j++){
                uint8_t data = j;
                send_data(0xa<<8 | data);
                hwlib::cout<<data<<" j\n";
                hwlib::wait_ms(speed);
            }

            //hwlib::wait_ms(500);

            for(int k = 15; k >= 0; k--){
                uint8_t data = k;
                send_data(0xa<<8 | data);
                hwlib::cout<<data<<" k\n";
                hwlib::wait_ms(speed);
            }
            hwlib::cout<<i<<" i------\n";
        }
    }
//brightness -----------------------------------------------------------------------------------------------    

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
    //leds.clear();
    hwlib::cout<<"---------------\n";
    
    hwlib::wait_ms(1000);

    //int letter_A [8] = {0x00, 0x7C, 0x7E, 0x13, 0x13, 0x7E, 0x7C, 0x00};
    //int green_lantern [8] = { 0x00, 0x99, 0xBD, 0xE7, 0xE7, 0xBD, 0x99, 0x00  };

    //leds.write(green_lantern);

    //hwlib::wait_ms(1000);

    int testing = 0b0000000101010101;
    leds.send_data(testing);



    //leds.write(letter_A, 2);







    hwlib::cout<<"done\n";
}