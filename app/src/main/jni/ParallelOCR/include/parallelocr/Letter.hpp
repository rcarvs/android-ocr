//
// Created by rcarvs on 16/02/17.
//
#include "../../../ParallelME/runtime/include/parallelme/ParallelME.hpp"
#include <parallelocr/ParallelOCR.hpp>
#include <cstring>
#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_LETTER_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_LETTER_HPP
namespace parallelocr{
class Letter{
    public:
        inline unsigned int getUpLimit(){
            return this->_upLimit;
        }
        inline unsigned int getDownLimit(){
            return this->_downLimit;
        }
        inline unsigned int getLeftLimit(){
            return this->_leftLimit;
        }
        inline unsigned int getRightLimit(){
            return this->_rightLimit;
        }
        inline void setUpLimit(unsigned int upLimit){
            this->_upLimit = upLimit;
        }
        inline void setDownLimit(unsigned int downLimit){
            this->_downLimit = downLimit;
        }
        inline void setLeftLimit(unsigned int leftLimit){
            this->_leftLimit = leftLimit;
        }
        inline void setRightLimit(unsigned int rightLimit){
            this->_rightLimit = rightLimit;
        }
        inline unsigned int* getLabels(){
            return this->_labels;
        }
        inline void setLabels(unsigned int *labels){
            this->_labels = labels;
        }
        inline unsigned int getLabelElement(int index){
            return this->_labels[index];
        }
        inline void setLabelElement(int index, unsigned int element){
            this->_labels[index] = element;
        }
        inline char* getCrossingRotule(){
            return this->_crossingRotule;
        }
        inline void setCrossingRotule(char *crossingRotule){
            this->_crossingRotule = crossingRotule;
        }

        std::string normalizeCrossingValue(unsigned int *ccount);
        void crossing(std::shared_ptr<parallelme::Runtime> runtime,std::shared_ptr<parallelme::Program> program);

    private:
        unsigned int _upLimit,_downLimit,_leftLimit,_rightLimit = 0;
        unsigned int *_labels;
        char *_crossingRotule;
};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_LETTER_HPP
