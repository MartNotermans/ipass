#include "hwlib.hpp"
#include "matrix.hpp"
#include "snake.hpp"

int main(){
    //Din   pin 8
    //cs    pin 9
    //clk   pin 10
    hwlib::wait_ms(500);

    auto swleft  = hwlib::target::pin_in (hwlib::target::pins::d36);
    auto swup    = hwlib::target::pin_in (hwlib::target::pins::d32);
    auto swdown  = hwlib::target::pin_in (hwlib::target::pins::d28);
    auto swright = hwlib::target::pin_in (hwlib::target::pins::d24);
    snake game(swleft, swup, swdown, swright); //scherm meegeven aan snake?
    game.play();
    hwlib::cout<<"done\n----------------\n";
}