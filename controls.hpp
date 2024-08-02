#pragma once
#include <SDL3/SDL_gamepad.h>

namespace Controls{
    //keeps track of the state of control inputs
    //if more than one controller is to be used, this will either need expanding or turning into an instantiable class
    SDL_Gamepad* findExistingGamepad();
    void pressButton(uint8_t button);
    void releaseButton(uint8_t button);
    void setAxis(uint8_t axis, int16_t value);
    void updateFlags();
}