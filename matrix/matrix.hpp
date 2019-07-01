#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "hwlib.hpp"
//Din   pin 8
//cs    pin 9
//clk   pin 10

/// @file
/// \brief
/// Class to write to a matrix
/// \details
/// This is a class to write to an 8*8 led matrix,
/// it is intended to be used with the MAX7219 chip.
/// this class uses hwlib (https://github.com/wovo/hwlib)
class matrix{
private:
    hwlib::target::pin_out din;
    hwlib::target::pin_out cs;
    hwlib::target::pin_out clk;
    int n_screens;

public:
    /// \brief
    /// Constructor to make a matrix
    /// \details
    /// This constructor makes a matrix with:
    /// data in pin,
    /// chip select pin,
    /// clock pin,
    /// 'x' number of screens.
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
    }

    /// \brief
    /// Initialise all screens
    /// \details
    /// This function does every thing to make all the screens operational.
    /// It sets the scanlimit to 8,
    /// sets decode mode off,
    /// sets brightness to low,
    /// turns shutdown off
    /// and clears all the screens.
    void set_up();

    /// \brief
    /// Pulse the clock once
    /// \details
    /// This function sets the clock high for 3 us (micro seconds),
    /// the total time of 1 clock pulse is 6 us.
    void pulse_clk();

    /// \brief
    /// Send 16 bits to 1 screen
    /// \details
    /// The first 8 bits are the adress, it says line we are writing on.
    /// The next 8 bits are on/off boolians.
    /// It uses the no_op function to sens the data to the right screen.
    void send_data(const int data, int screen = 1);

    /// \brief
    /// Send a dubbel array of boolians to 1 screen
    /// \details
    /// The function reads an 8*8 array and
    /// uses the send_data function to send this to 1 screen.
    void send_array(bool pixels[8][8], int screen);

    /// \brief
    /// Send 16 zero's as no op
    /// \details
    /// The MAX7219 is desined to ignore no_op data.
    /// This is so you can push data to daisy changed screens
    void no_op(int times = 1);

    /// \brief
    /// Clear 1 screen
    /// \details
    /// Send an empty screen using the send_array function
    void clear(int screen);

    /// \brief
    /// Flash all screens
    /// \details
    /// Flash all screens by changing the brightness 'x' times at 'y' speed
    void flash(int times = 3, int speed = 150);
};
#endif