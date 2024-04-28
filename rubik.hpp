#include <stdint.h>
Class Rubik {
    private:
        //list of squares in each path around the cube
        //see notebook diagram for indices
        static uint8_t paths[9][12] = {
            { 0, 7, 6, 9,16,15,36,43,42,45,52,51},
            { 1, 8, 5,10,17,14,37,44,41,46,53,50},
            {49,48,47,40,39,38,13,12,11, 4, 3, 2},
            {51,50,49,29,28,27,11,10, 9,20,19,18},
            {52,53,48,30,35,34,12,17,16,21,26,25},
            {47,46,45,24,23,22,15,14,13,33,32,31},
            { 6, 5, 4,27,34,33,38,37,36,22,21,20},
            { 7, 8, 3,28,35,32,39,44,43,23,26,19},
            {18,25,24,42,41,40,31,30,29, 2, 1, 0}};
        //face each path is associated with
        static uint8_t face[9] = {1,6,3,0,6,4,2,6,5};
        //default side colours (white, red, blue, green, orange, purple)
        uint8_t colours[6][3] = {
            {255,255,255},
            {255,  0,  0},
            {  0,  0,255},
            {  0,255,  0},
            {255,127,  0},
            {255,  0,255}};
        //colour (0-5) of each square on the cube
        uint8_t squares[54] = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5};
    public:
        Rubik(uint8_t* 6rgb){
            //if Rubik is initialised with nullptr argument, use default colour values
            if(6rgb != nullptr){
                //copy 18 bytes (6 triplets of RGB 0-255 values) into the colour array
                memcpy(colours, 6rgb, 18)
            }
        }
        void rotateFace(uint8_t f, uint8_t d){
            //function to rotate a face when a track running around it is twisted
            //f is the face to be rotated
            //d is whether the face is rotated clockwise (0) or anticlockwise (1)
            if(d == 0){
                uint8_t store = this.squares[(9*f)+7];
                for(uint8_t i = (9 * f) + 7; i > 9 * f; i--){
                    this.squares[i] = this.squares[i-1];
                }
                this.squares[9*f] = store;
            }
            else{
                uint8_t store = this.squares[9*f];
                for(uint8_t i = 9 * f; i < (9 * f) + 7; i++){
                    this.squares[i] = this.squares[i+1];
                }
                this.squares[(9*f)+7] = store;
            }
        }
        void rotatePath(uint8_t p, uint8_t d){
            //function to rotate a path of squares
            //p is the path to be rotated (0-8)
            //d is whether the path is rotated forwards (0) or backwards (1)
            //this function calls rotateFace - the d parameter is passed to it unmodified
            ;
        }
        ~Rubik(){
            ;
        }
};