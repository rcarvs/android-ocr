//
// Created by rcarvs
//
#include <unistd.h>
#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_RGB_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_RGB_HPP
namespace parallelocr{
class RGB{
    public:
        RGB();
        RGB(uint32_t pixel);
        RGB(int red,int green,int blue): _red(red),_green(green),_blue(blue){}

        inline void setRed(unsigned int red){
            this->_red = red;
        }
        inline unsigned int getRed(){
            return this->_red;
        }
        inline void setGreen(unsigned int green){
            this->_green = green;
        }
        inline unsigned int getGreen(){
            return this->_green;
        }
        inline void setBlue(unsigned int blue){
            this->_blue = blue;
        }
        inline unsigned int getBlue(){
            return this->_blue;
        }



    private:

        unsigned int _red,_green,_blue;
};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_RGB_HPP
