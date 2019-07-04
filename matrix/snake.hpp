//          Copyright Mart Notermans 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "matrix.hpp"
#include "hwlib.hpp"

/// @file
/// \brief
/// Class to play snake
/// \details
/// clas toplay snake using the matrix class and hwlib
/// (https://github.com/wovo/hwlib)
class snake{
private:
    hwlib::target::pin_in swleft;
    hwlib::target::pin_in swup;
    hwlib::target::pin_in swdown;
    hwlib::target::pin_in swright;

    matrix leds;

    /// \brief
    /// empty 2d boolean array for an empty screen
    bool empty_screen[8][8] = {
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
    
    /// \brief
    /// 3d boolean array for the screen
    /// \details
    /// this array gets updated with the new frame
    /// at the end of the loop the array gets printed using the send_array function from matrix
    bool screens[6][8][8] = {
                            {empty_screen},
                            {empty_screen},
                            {empty_screen},
                            {empty_screen},
                            {empty_screen},
                            {empty_screen}
                            };//screen, y, x

    /// \brief
    /// 4 booleans to remember which button was pressed last
    /// sw1 = left
    /// sw2 = up
    /// sw3 = down
    /// sw4 = right
    bool sw1;   //left
    bool sw2;   //up
    bool sw3;   //down
    bool sw4;   //right

    /// \brief
    /// the speed of the game
    /// \details
    /// every time you get a point the game gets faster
    int update_speed = 250;

    /// \brief
    /// int to indicate which way the snake is currently going
    /// 1 = left
    /// 2 = up
    /// 3 = down
    /// 4 = right
    int way;

    /// \brief
    /// coordinates of the apple
    /// \details
    /// touch the apple and you get a point
    int applecords[3] = {0,0,0};

    /// \brief
    /// number of points you have
    /// \details
    /// your points are also the length of the snake
    /// 0 points is only the head
    int points = 0;

    /// \brief
    /// array of cordinates of the snake
    /// \details
    /// the max array size is 8*8*6 (384)
    // this is the number of pixels so the max possible score
    int tail[ (8*8*6) ][3]; //coordinates of each point

    bool playing = 1;

public:
    snake(  hwlib::target::pin_in swleft,
            hwlib::target::pin_in swup,
            hwlib::target::pin_in swdown,
            hwlib::target::pin_in swright,
            matrix& leds):
            swleft(swleft), swup(swup), swdown(swdown), swright(swright), leds(leds)
            {
                //snake head position
                tail[0][0] = 0;
                tail[0][1] = 0;
                tail[0][2] = 0;

                
                //clear top left corner on every screen
                for(int i = 0; i <= 5; i++){
                    int temp[3] = {0,0,i};
                    set_screen(temp, 0);
                }

                apple(); //first apple
                sw1 = 0;
                sw2 = 0;
                sw3 = 0;
                sw4 = 1; //start by going left
            }

    /// \brief
    /// get a boolean from a pin
    /// \details
    /// used to read the buttons to control the snake's movement
    bool get_pin(hwlib::target::pin_in& button);

    /// \brief
    /// check which button is last pressed
    /// \details
    /// changes the 4 sw booleans to the right value
    /// changes the way int to the right value
    void button();

    /// \brief
    /// compare 2 array[3]
    /// \details
    /// retrun true if 2 coordinates are the same
    bool equals(int lhs[3], int rhs[3]);

    /// \brief
    /// set a coordinate on or off
    /// \details
    /// sets a coordinate on or of in the screen array
    /// set the screen before sending the data
    void set_screen(int cords[3], bool state);

    /// \brief
    /// write the screen array to the matrix using send array
    void write_screen(matrix& leds);

    /// \brief
    /// update coordinates based on movement
    /// \details
    /// updates the position of a coordinate in a given direction
    /// used to update tail[0], the head of the snake
    void update_cords(int cords[3], int temp_way);

    /// \brief
    /// check if a cord is going to move to another screen
    /// \details
    /// this function checks if the given cordinate is going to move to another screen
    /// if this is the case, the function will change the coordinates to the next screen
    void next_screen(int cords[3]);

    /// \brief
    /// make a new apple
    /// \details
    /// updates the cordinates of apple to a new random position on the screen
    void apple();

    /// \brief
    /// endless loop with game logic
    /// \details
    /// call this function to start the game
    /// it has an endless loop which calls all the other functions in orther to play the game
    void play();

    /// \brief
    /// flash game over and stop the program
    /// \details
    /// the play function calls this function if the game is over
    /// it will flash game over and stop the program
    void game_over(matrix& leds);
};

#endif