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
            return SDL_OpenGamepad(i);
        }
    }
    printf("No controllers connected.\n");
    return nullptr;
}
bool Controls::edge(uint8_t button){
    if(buttons_current[button] != buttons_previous[button]){
        return true;
    }
    else{
        return false;
    }
}
bool Controls::fallingEdge(uint8_t button){
    if(!buttons_current[button] && buttons_previous[button]){
        return true;
    }
    else{
        return false;
    }
}
int16_t* Controls::getAxes(){
    return axes_current;
}
int16_t Controls::getAxis(uint8_t axis){
    return axes_current[axis];
}
bool Controls::getButton(uint8_t button){
    return buttons_current[button];
}
bool* Controls::getButtons(){
    return buttons_current;
}
int16_t* Controls::getPreviousAxes(){
    return axes_previous;
}
int16_t Controls::getPreviousAxis(uint8_t axis){
    return axes_previous[axis];
}
bool Controls::getPreviousButton(uint8_t button){
    return buttons_previous[button];
}
bool* Controls::getPreviousButtons(){
    return buttons_previous;
}
void Controls::pressButton(uint8_t button){
    if(button < 26 && button != SDL_GAMEPAD_BUTTON_INVALID){
        buttons_current[button] = true;
    }  
}
bool Controls::risingEdge(uint8_t button){
    if(buttons_current[button] && !buttons_previous[button]){
        return true;
    }
    else{
        return false;
    }
}
void Controls::releaseButton(uint8_t button){
    if(button < 26 && button != SDL_GAMEPAD_BUTTON_INVALID){
        buttons_current[button] = false;
    }
}
void Controls::setAxis(uint8_t axis, int16_t value){
    if(axis < 7 && axis != SDL_GAMEPAD_AXIS_INVALID){
        axes_current[axis] = value;
    }
}
void Controls::updateFlags(){
    memcpy(axes_previous, axes_current, 7 * sizeof(int16_t));
    memcpy(buttons_previous, buttons_current, 26 * sizeof(bool));
} 