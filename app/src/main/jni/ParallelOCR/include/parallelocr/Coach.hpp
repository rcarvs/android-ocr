//
// Created by rcarvs
//
#include <jni.h>
#include <parallelocr/ParallelOCR.hpp>
#include <sys/types.h>
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


        //void Train(char letter,unsigned int labelsCount,unsigned int *labels);
        inline void setEnv(JNIEnv *env){
            this->_env = env;
        }
        inline AAssetManager* getAssetManager(){
            return this->_assetManager;
        }
        inline void setAssetManager(AAssetManager *assetManager){
            this->_assetManager = assetManager;
        }
    private:
        JNIEnv *_env;
        AAssetManager *_assetManager;
        //Player *_players;




};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_COACH_HPP
