//
// Created by Renan Carvalho
//
#include <parallelocr/Coach.hpp>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
using namespace parallelocr;


Coach::Coach(JNIEnv *env,jobject javaAssetManager){


    this->setEnv(env);
    this->setAssetManager(AAssetManager_fromJava(env, javaAssetManager));
    AAsset* openInfo = AAssetManager_open(this->getAssetManager(),"coach/data.txt",AASSET_MODE_BUFFER);
    if(!openInfo){
        __android_log_print(ANDROID_LOG_INFO, "Log Error", "Not was possible open the info.txt");
        return;
    }
    size_t infoLenght = AAsset_getLength(openInfo);
    char* infoBuffer = (char*) malloc(infoLenght + 1);
    AAsset_read(openInfo, infoBuffer, infoLenght);
    infoBuffer[infoLenght] = 0;
    this->_file = infoBuffer;

    this->_dataSize = 0;
    for(unsigned int i=0;i<infoLenght;i++){
        if(isdigit(this->_file[i]) || isalpha(this->_file[i])){
            this->_dataSize++;
        }
    }
    this->_data = (unsigned int*) malloc(sizeof(unsigned int)*this->_dataSize);
    int count =0;
    for(unsigned int i=0;i<infoLenght;i++){
        if(isdigit(this->_file[i])){
            this->_data[count] = this->_file[i]-'0';
            count++;
        }else if(isalpha(this->_file[i])){
            this->_data[count] = this->_file[i]-'0';
            count++;
        }
    }

}