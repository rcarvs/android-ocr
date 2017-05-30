//
// Created by Renan Carvalho
//
#include <jni.h>
#include <parallelocr/ParallelOCR.hpp>
#include <sys/types.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_COACH_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_COACH_HPP

namespace parallelocr{
class Coach{
    public:
        /*
        The objective is get the player structure in file and put in the set of players class
        */

        Coach(JNIEnv *env,jobject javaAssetManager);

        inline void setEnv(JNIEnv *env){
            this->_env = env;
        }
        inline AAssetManager* getAssetManager(){
            return this->_assetManager;
        }

        inline void setAssetManager(AAssetManager *assetManager){
            this->_assetManager = assetManager;
        }

        std::string _file;
        unsigned int *_data;
        unsigned int _dataSize;
        char _alfabhet[26] ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','K','R','S','T','U','V','W','X','Y','Z'};

        //Evaluation
        unsigned int _count_evaluation = 0;
        unsigned int _count_labels = 0;
        double *_searchLetter;
        double *_buffers;
        double *_tasks;
        double *_submission;
        double *_finish;
        double *_result;

    private:
        JNIEnv *_env;
        AAssetManager *_assetManager;

};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_COACH_HPP
