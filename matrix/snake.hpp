#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "matrix.hpp"
#include "hwlib.hpp"

class snake{
private:
    hwlib::target::pin_in swleft;
    hwlib::target::pin_in swup;
    hwlib::target::pin_in swdown;
    hwlib::target::pin_in swright;
public:
    snake(  hwlib::target::pin_in swleft,
            hwlib::target::pin_in swup,
            hwlib::target::pin_in swdown,
            hwlib::target::pin_in swright):
            swleft(swleft), swup(swup), swdown(swdown), swright(swright)
            {}

    bool get_pin(hwlib::target::pin_in& button);

    void play(matrix& leds);

    void game_over(matrix& leds);
};

#endif