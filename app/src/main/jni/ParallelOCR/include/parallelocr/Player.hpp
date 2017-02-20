//
// Created by rcarvs on 19/02/17.
//

#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_PLAYER_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_PLAYER_HPP
namespace parallelocr{
class Player{
    public:
        Player();

        inline void setLetter(char *letter){
            this->_letter = letter;
        }
        inline void setLabels(unsigned int *labels){
            this->_labels = labels;
        }
        inline void setLabel(int index,unsigned int label){
            this->_labels[index] = label;
        }

        inline void setLabelsCount(unsigned int labelsCount){
            this->_labelsCount = labelsCount;
        }

        inline char* getLetter(){
            return this->_letter;
        }
        inline unsigned int getLabel(int index){
            return this->_labels[index];
        }
        inline unsigned int* getLabels(){
            return this->_labels;
        }
        inline unsigned int getLabelsCount(){
            return this->_labelsCount;
        }
        char *_letter;
    private:
        unsigned int *_labels;
        unsigned int _labelsCount;
};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_PLAYER_HPP
