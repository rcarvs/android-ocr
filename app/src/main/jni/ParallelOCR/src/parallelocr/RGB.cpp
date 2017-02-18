//
// Created by rcarvs
//

#include <parallelocr/RGB.hpp>
using namespace parallelocr;
RGB::RGB(uint32_t pixel){
    this->setRed((unsigned int) ((pixel & 0x00FF0000) >> 16));
    this->setGreen((unsigned int) ((pixel & 0x0000FF00) >> 8));
    this->setBlue((unsigned int) ((pixel & 0x00000FF)));
}
