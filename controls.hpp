#pragma once
#include <SDL3/SDL_gamepad.h>

namespace Controls{
    //keeps track of the state of control inputs
    //if more than one controller is to be used, this will either need expanding or turning into an instantiable class
    SDL_Gamepad* findExistingGamepad();
    bool edge(uint8_t button);
    bool fallingEdge(uint8_t button);
    int16_t* getAxes();
    int16_t getAxis(uint8_t axis);
    bool getButton(uint8_t button);
    bool* getButtons();
    int16_t* getPreviousAxes();
    int16_t getPreviousAxis(uint8_t axis);
    bool getPreviousButton(uint8_t button);
    bool* getPreviousButtons();
    void pressButton(uint8_t button);
    bool risingEdge(uint8_t button);
    void releaseButton(uint8_t button);
    void setAxis(uint8_t axis, int16_t value);
    void updateFlags();
}