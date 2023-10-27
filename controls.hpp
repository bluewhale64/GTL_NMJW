struct ControllerStatus {
    //'true' means the button is ALREADY being held down, and 'false' means it isn't.
    //P and M correspond to Plus and Minus.
    bool FLAG_A, FLAG_B, FLAG_X, FLAG_Y, FLAG_L, FLAG_R, FLAG_P, FLAG_M, FLAG_ZL, FLAG_ZR, FLAG_DU, FLAG_DD, FLAG_DL, FLAG_DR, FLAG_LXP, FLAG_LXM, FLAG_LYP, FLAG_LYM, FLAG_RXP, FLAG_RXM, FLAG_RYP, FLAG_RYM = false;
    //'true' means the button is being held down, and 'false' means it isn't.
    bool A, B, X, Y, L, R, P, M, DU, DD, DL, DR;
    //analogue sticks and triggers vary between -32768 and +32767.
    Sint16 LX, LY, RX, RY;
    //ZL and ZR are regarded by SDL as analogue shoulder triggers, like a GameCube controller.
    Sint16 ZL, ZR;
};

class Controller {
    private:
        //Flag for toggling fullscreen
        bool FLAG_F11;
        //SDL event to handle key presses and controllers
        SDL_Event event;
        //pointer to an SDL controller gamepad
        SDL_GameController* gamepad;
        //The threshold for the joystick detecting movement - allows the joystick to be nudged a tiny bit without triggering motion.
        //A positive float between 0 and 32768.
        //Setting THR to 32767 means the joystick will never detect movement, and setting it to 0 means the joystick will detect every movement.
        Sint16 THR = 4096; //An eighth of the maximum
        ControllerStatus status;
    public:
        Controller(){
            ;
        }
        void updatestatus(){
            //First, copy the current values into their corresponding flags.
            status.FLAG_A   = status.A;
            status.FLAG_B   = status.B;
            status.FLAG_X   = status.X;
            status.FLAG_Y   = status.Y;
            status.FLAG_L   = status.L;
            status.FLAG_R   = status.R;
            status.FLAG_P   = status.P;
            status.FLAG_M   = status.M;
            status.FLAG_ZL  = status.ZL;
            status.FLAG_ZR  = status.ZR;
            status.FLAG_DU  = status.DU;
            status.FLAG_DD  = status.DD;
            status.FLAG_DL  = status.DL;
            status.FLAG_DR  = status.DR;
            //Joystick values need a bit of a different approach.
            status.FLAG_LXP = status.LX >  THR;
            status.FLAG_LXM = status.LX < -THR;
            status.FLAG_LYP = status.LY >  THR;
            status.FLAG_LYM = status.LY < -THR;
            status.FLAG_RXP = status.RX >  THR;
            status.FLAG_RXM = status.RX < -THR;
            status.FLAG_RYP = status.RY >  THR;
            status.FLAG_RYM = status.RY < -THR;
            //Then, read the status of the buttons and joysticks.
            status.A  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_A);
            status.B  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_B);
            status.X  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_X);
            status.Y  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_Y);
            status.L  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_LEFTSTICK);
            status.R  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
            status.P  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_START);
            status.M  = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_BACK);
            status.DU = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_UP);
            status.DD = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
            status.DL = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            status.DR = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
            status.LX = SDL_GameControllerGetAxis  (gamepad, SDL_CONTROLLER_AXIS_LEFTX);
            status.LY = SDL_GameControllerGetAxis  (gamepad, SDL_CONTROLLER_AXIS_LEFTY);
            status.RX = SDL_GameControllerGetAxis  (gamepad, SDL_CONTROLLER_AXIS_RIGHTX);
            status.RY = SDL_GameControllerGetAxis  (gamepad, SDL_CONTROLLER_AXIS_RIGHTY);
            status.ZL = SDL_GameControllerGetAxis  (gamepad, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            status.ZR = SDL_GameControllerGetAxis  (gamepad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        }
        ControllerStatus getstatus(){
            return status;
        }
        bool keycheck(bool* ESC){
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        *ESC = true;
                    }
                    if (event.key.keysym.sym == SDLK_F11) {
                        if(FLAG_F11 == false){
                            FLAG_F11 = true;
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                }
                if(event.type == SDL_KEYUP){
                    if (event.key.keysym.sym == SDLK_F11){
                        FLAG_F11 = false;
                        return false;
                    }
                }
                else{
                    return false;
                }
            }
            return false;
        }
        ~Controller(){
            ;
        }
};