//          Copyright Mart Notermans 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



#include "hwlib.hpp"
#include "matrix.hpp"
#include "snake.hpp"

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



    auto swleft  = hwlib::target::pin_in (hwlib::target::pins::d36);
    auto swup    = hwlib::target::pin_in (hwlib::target::pins::d32);
    auto swdown  = hwlib::target::pin_in (hwlib::target::pins::d28);
    auto swright = hwlib::target::pin_in (hwlib::target::pins::d24);
    snake game(swleft, swup, swdown, swright, leds);
    game.play();
    hwlib::cout<<"done";
}