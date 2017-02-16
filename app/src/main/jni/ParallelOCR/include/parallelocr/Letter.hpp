//
// Created by rcarvs on 16/02/17.
//

#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_LETTER_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_LETTER_HPP
namespace parallelocr{
class Letter{
    public:
        inline int getUpLimit(){
            return this->_upLimit;
        }
        inline int getDownLimit(){
            return this->_downLimit;
        }
        inline int getLeftLimit(){
            return this->_leftLimit;
        }
        inline int getRightLimit(){
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
    private:
        unsigned int _upLimit,_downLimit,_leftLimit,_rightLimit = 0;
};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_LETTER_HPP
