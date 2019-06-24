#include <hwlib.hpp>

//void test(const int data_b){
//    uint8_t data = data_b;
//    data = data<<8;
//    for(int8_t bit_cnt = 7; bit_cnt >= 0; bit_cnt--){
//        bool print( (data & (1<<bit_cnt) ) !=0 );
//        hwlib::cout<<print;
//    }
//    hwlib::cout<<"\n";
//}

void test2(){
    uint8_t data8 = 3;
    uint16_t data16 = ( (data8<<8) | 0x07);
    for(int16_t bit_cnt = 15; bit_cnt >= 0; bit_cnt--){
        bool print( (data16 & (1<<bit_cnt) ) !=0 );
        hwlib::cout<<print;
    }
    hwlib::cout<<"\n";
}

int main(){
    //test2();
    int number = 1*4;
    hwlib::cout<<number;
}
