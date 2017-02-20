//
// Created by rcarvs on 19/02/17.
//
#include <parallelocr/Coach.hpp>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
using namespace parallelocr;
Coach::Coach(JNIEnv *env,jobject javaAssetManager){
    this->setEnv(env);
    this->setAssetManager(AAssetManager_fromJava(env, javaAssetManager));
    AAsset* openInfo = AAssetManager_open(this->getAssetManager(),"coach/data.txt",AASSET_MODE_BUFFER);
    if(!openInfo){
        //show a error message
        __android_log_print(ANDROID_LOG_INFO, "Log Error", "Not was possible open the info.txt");
        return;
    }
    size_t infoLenght = AAsset_getLength(openInfo);
    char* infoBuffer = (char*) malloc(infoLenght + 1);
    AAsset_read(openInfo, infoBuffer, infoLenght);
    infoBuffer[infoLenght] = 0;
    char *explode;
    explode = strtok(infoBuffer,"\n");
    int countLines;
    sscanf(explode,"%d",&countLines);
    __android_log_print(ANDROID_LOG_INFO, "Log Cpp", "%d",countLines);
    explode = strtok(NULL,"\n");
    while(explode != NULL){
        //char* line = explode;
        //char *part = strtok(line," ");
        //__android_log_print(ANDROID_LOG_INFO, "Log Cpp", "%d",((int)part));
        //__android_log_print(ANDROID_LOG_INFO, "Log Cpp", "%s",explode);
        explode = strtok(NULL,"\n");
    }


};