#include <hwlib.hpp>
//Din   pin 11
//cs    pin 12
//clk   pin 13

class spi_ding{
public:
    hwlib::target::pin_out din;
    hwlib::target::pin_out cs;
    hwlib::target::pin_out clk;

    spi_ding(hwlib::target::pin_out din, hwlib::target::pin_out cs, hwlib::target::pin_out clk):
    din (din), cs (cs), clk (clk)
    {
        auto spi  = hwlib::spi_bus_bit_banged_sclk_mosi_miso (clk, din, hwlib::pin_in_dummy);
    }
};

int main(){
    hwlib::wait_ms(1000);
    
    auto din = hwlib::target::pin_out( hwlib::target::pins::d11 );
    auto cs = hwlib::target::pin_out( hwlib::target::pins::d12 );
    auto clk = hwlib::target::pin_out( hwlib::target::pins::d13 );
}