#pragma once
#include <SDL3/SDL_gamepad.h>

/// @brief #include "controls.hpp"
///
/// Keeps track of the state of controller inputs.
///
/// If multiple controllers are to be used, this will need expanding.
///
/// This can be done by:
///
/// - adding separate arrays for the states of a second controller.
///
/// - turning the arrays into two dimensional structures, with the new dimension used to specifiy which player's controls are being accessed.
///
/// - rewriting the namespace into an instantiable class.
namespace Controls{
    /// @brief Checks if a given button is changing state on this frame.
    /// @param button The button in question.
    ///
    /// Uses the SDL_GamepadButton enum.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    /// @return true if the button is changing state, and false if it isn't.
    bool edge(uint8_t button);
    /// @brief Checks if a given button has been released on this frame.
    /// @param button The SDL_GamepadButton enum name of the button.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    /// @return true if the button is being released, and false if it isn't.
    bool fallingEdge(uint8_t button);
    /// @brief Checks if there already is an active and connected controller.
    ///
    /// This will only connect to the first controller the program finds.
    /// @return A pointer to the controller handle if there is one, and nullptr if there isn't.
    SDL_Gamepad* findExistingGamepad();
    /// @brief View several axis states at once.
    /// @return A pointer to all the axis states.
    ///
    /// Access individual axes using the SDL_GamepadAxis enum.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadAxis
    int16_t* getAxes();
    /// @brief Get the value of one joystick axis.
    /// @param axis the SDL_GamepadAxis enum name of the axis.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadAxis
    /// @return The value of the axis.
    int16_t getAxis(uint8_t axis);
    /// @brief Gets the stats of a given controller button.
    /// @param button The SDL_GamepadButton enum name of the button.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    /// @return true if the button is currently pressed down, and false if it isn't.
    bool getButton(uint8_t button);
    /// @brief View several button states at once.
    /// @return A pointer to all the button states.
    ///
    /// Access individual buttons using the SDL_GamepadButton enum.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    bool* getButtons();
    /// @brief View several axis states from the previous frame at once.
    /// @return A pointer to all the axis states from the previous frame.
    ///
    /// Access individual axes using the SDL_GamepadAxis enum.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadAxis
    int16_t* getPreviousAxes();
    /// @brief Get the state of an axis from the previous frame.
    /// @param button The SDL_GamepadAxis enum name of the axis.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadAxis
    /// @return the value of the axis during the previous frame.
    int16_t getPreviousAxis(uint8_t axis);
    /// @brief Get the state of a button from the previous frame.
    /// @param button The SDL_GamepadButton enum name of the button.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    /// @return true if the button was pressed down last frame, and false if it wasn't.
    bool getPreviousButton(uint8_t button);
    /// @brief View several button states from the previous frame at once.
    /// @return A pointer to all the button states from the previous frame.
    ///
    /// Access individual axes using the SDL_GamepadButton enum.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    bool* getPreviousButtons();
    /// @brief Registers that a particular controller button has been pressed.
    /// @param button The SDL_GamepadButton enum name of the button.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    void pressButton(uint8_t button);
    /// @brief Checks if a given button has been pressed on this frame.
    /// @param button The SDL_GamepadButton enum name of the button.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    /// @return true if the button is being pressed, and false if it isn't.
    bool risingEdge(uint8_t button);
    /// @brief Registers that a particular controller button has been released.
    /// @param button The SDL_GamepadButton enum name of the button.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
    void releaseButton(uint8_t button);
    /// @brief Registers that a particular controller axis has been moved.
    /// @param axis The SDL_GamepadAxis enum name of the axis.
    ///
    /// https://wiki.libsdl.org/SDL3/SDL_GamepadAxis
    /// @param value The signed 2-byte integer value of the controller axis position.
    void setAxis(uint8_t axis, int16_t value);
    /// @brief Overwrites the states of controller inputs from the previous frame with those of the current frame.
    ///
    /// This enables comparisons between the controller states on this frame and the next frame.
    ///
    /// This function should only be called once: either at the very start or the very end of the main loop.
    void updateFlags();
}