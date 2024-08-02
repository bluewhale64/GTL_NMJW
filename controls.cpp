#include "controls.hpp"
#include <inttypes.h>
#include <cstring>
#include <cstdio>
#include <SDL3/SDL_main.h>

namespace {
    //follows enums SDL_GamepadButton and SDL_GamepadAxis
    //access members using the enum names - see https://wiki.libsdl.org/SDL3/SDL_GamepadButton and https://wiki.libsdl.org/SDL3/SDL_GamepadAxis
    int16_t axes_current[7] = {0};
    int16_t axes_previous[7] = {0};
    bool buttons_current[26] = {false};
    bool buttons_previous[26] = {false};
    //Add arrays for pressed keys??
}
SDL_Gamepad* Controls::findExistingGamepad(){
    int number_of_joysticks;
    SDL_GetJoysticks(&number_of_joysticks);
    for (int i = 0; i < number_of_joysticks; i++){
        if (SDL_IsGamepad(i)){
            printf("Controller detected.\n");
            //connect to first detected controller only
            return SDL_OpenGamepad(i);;
        }
    }
    printf("No controllers connected.\n");
    return nullptr;
}
void Controls::pressButton(uint8_t button){
    if(button < 26 && button > -1){
        buttons_current[button] = true;
    }   
}
void Controls::releaseButton(uint8_t button){
    if(button < 26 && button > -1){
        buttons_current[button] = false;
    }
}
void Controls::setAxis(uint8_t axis, int16_t value){
    if(axis < 7 && axis > -1){
        axes_current[axis] = value;
    }
}
void Controls::updateFlags(){
    memcpy(axes_previous, axes_current, 7 * sizeof(int16_t));
    memcpy(buttons_previous, buttons_current, 26 * sizeof(bool));
} 