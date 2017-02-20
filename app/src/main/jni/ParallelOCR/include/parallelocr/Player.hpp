//
// Created by rcarvs on 19/02/17.
//

#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_PLAYER_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_PLAYER_HPP
namespace parallelocr{
class Player{
    public:
        Player();
        Player(char letter,unsigned int labelsCount,unsigned int *labels):_letter(letter),_labels(labels),_labelsCount(labelsCount){}
        inline void setLetter(char letter){
            this->_letter = letter;
        }
        inline void setLabels(unsigned int *labels){
            this->_labels = labels;
        }
        inline void setLabelsCount(unsigned int labelsCount){
            this->_labelsCount = labelsCount;
        }
        inline char getLetter(){
            return this->_letter;
        }
        inline unsigned int* getLabels(){
            return this->_labels;
        }
        inline unsigned int getLabelsCount(){
            return this->_labelsCount;
        }
    private:
        char _letter;
        unsigned int *_labels;
        unsigned int _labelsCount;
};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_PLAYER_HPP
