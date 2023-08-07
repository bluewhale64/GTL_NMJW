struct ProControl {
    //Written with a Switch Pro Controller in mind:
    //P, M and H correspond to Plus, Minus and Home.
    bool A, B, X, Y, L, R, ZL, ZR, P, M, H;
    float LX, LY, RX, RY;
    //In case tilt data needs to be used:
    //float LGX, LGY, LGZ, RGX, RGY, RGZ;
    //float CGX, CGY, CGZ;
};

class Controller {
    private:
        bool left, right, up, down;
        bool flag_l, flag_r, flag_u, flag_d;
        ProControl status;
    public:
        Controller(){
            ;
        }
        void updatestatus(){
            ;
        }
        ProControl getstatus(){
            return status;
        }
        ~Controller(){
            ;
        }
};