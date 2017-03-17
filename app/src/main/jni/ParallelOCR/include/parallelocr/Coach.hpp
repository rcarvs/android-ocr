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
        inline Player getPlayer(int index){
            return this->_players[index];
        }
        inline Player* getPlayers(){
            return this->_players;
        }
        inline void setPlayers(Player *players){
            this->_players = players;
        }
        inline void setAssetManager(AAssetManager *assetManager){
            this->_assetManager = assetManager;
        }
        inline void setPlayersCount(unsigned int playersCount){
            this->_playersCount = playersCount;
        }
        inline unsigned int getPlayersCount(){
            return this->_playersCount;
        }
        Player *_players;
        std::string _file;
        unsigned int *_data;
        unsigned int _dataSize;
        char _alfabhet[26] ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','K','R','S','T','U','V','W','X','Y','Z'};
    private:
        JNIEnv *_env;
        AAssetManager *_assetManager;

        unsigned int _playersCount;




};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_COACH_HPP
