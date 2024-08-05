#include "controls.hpp"
#include <inttypes.h>
#include <cstring>
#include <cstdio>
#include <SDL3/SDL_main.h>

/// @brief This anonymous (unnamed) namespace holds the arrays of controller states.
///
/// It can't be accessed by functions written outsie of this file (controls.cpp).
///
/// The functions here can access the data and pass it to code outside this file.
namespace {
    /// @brief  the state of all the controller axes on the current frame.
    int16_t axes_current[7] = {0};
    /// @brief  the state of all the controller axes from the previous frame.
    int16_t axes_previous[7] = {0};
    /// @brief  the state of all the controller buttons on the current frame.
    bool buttons_current[26] = {false};
    /// @brief  the state of all the controller buttons from the previous frame.
    bool buttons_previous[26] = {false};
    //Add arrays for pressed keys??
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