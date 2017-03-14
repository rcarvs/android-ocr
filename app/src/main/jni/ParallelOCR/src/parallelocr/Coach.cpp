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
    //first i count the number
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
    for(int i=0; i<count;i++){
        __android_log_print(ANDROID_LOG_INFO, "Log Test", "%d",this->_data[i]);
    }
    /*char *explode;
    explode = strtok(infoBuffer,"\n");
    int countLines;
    sscanf(explode,"%d",&countLines);
    this->_players = (Player*) malloc(sizeof(Player*)*countLines);
    this->setPlayersCount(countLines);
    explode = strtok(NULL,"\n");

    int count = 0;
    while(explode != NULL){
        char letter[2];

        unsigned int labelsCount = 0;
        sscanf(explode,"%s %d",letter,&labelsCount);


        this->_players[count].setLabelsCount(labelsCount);
        this->_players[count].setLetter(letter);
        this->_players[count].setLabels((unsigned int*) malloc(sizeof(unsigned int)*labelsCount));
        explode = strstr(explode," ")+1;
        explode = strstr(explode," ")+1;

        for(unsigned int i=0;i<labelsCount;i++){
            unsigned int temp;
            sscanf(explode,"%d ",&temp);
            this->_players[count].setLabel(i,temp);
            explode = strstr(explode," ")+1;
        }

        __android_log_print(ANDROID_LOG_INFO, "Log Error", "LC: %d Letter: %s - %s",labelsCount,letter,this->_players[count]._letter);


        explode = strtok(NULL,"\n");
        count++;
    }
    */



};