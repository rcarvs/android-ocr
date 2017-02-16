//
// Created by rcarvs
//
#include <parallelocr/RGB.hpp>
#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_PIXEL_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_PIXEL_HPP
namespace parallelocr{
class Pixel{
    public:
        Pixel(RGB rgb);

        inline RGB getRGB(){
            return this->_rgb;
        }
        inline unsigned int getLabel(){
            return this->_label;
        }

        inline void setRGB(RGB rgb){
            this->_rgb = rgb;
        }

        inline void setLabel(unsigned int label){
            this->_label = label;
        }

        inline bool getChecked(){
            return this->_checked;
        }
        inline void setChecked(bool checked){
            this->_checked = checked;
        }

    private:
        RGB _rgb;
        unsigned int _label;
        bool _checked;
};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_PIXEL_HPP
