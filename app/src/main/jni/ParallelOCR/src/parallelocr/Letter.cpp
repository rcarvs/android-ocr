//
// Created by Renan Carvalho
//
#include <parallelocr/Letter.hpp>
#include <android/log.h>
#include <string>

using namespace parallelocr;

/*bool Letter::letterIdentification(std::shared_ptr<Coach> coach){
    for(unsigned int i =0;i< coach->getPlayersCount();i++){
        //First compare the number of labels
        //__android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%s - %d",coach->getPlayer(i)._letter,coach->getPlayer(i).getLabelsCount());
        if(coach->getPlayer(i).getLabelsCount() == this->_crossingRotuleSize){
            __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "ENTROU AQUI");
            //bool equals = true;
            for(unsigned int j=0;j<this->_crossingRotuleSize;j++){
                __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Testeeee %d %d - %d",this->_crossingRotule[j],coach->getPlayer(i).getLabel(j),coach->getPlayer(i)._letter);
            }
            if(equals){
                __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%d Acertou ",coach->getPlayer(i)._letter);

                //this->_letter[1] = coach->getPlayer(i)._letter[1];
                return true;
            }
        }
    }

    return false;
}*/

inline bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

void Letter::crossing(std::shared_ptr<parallelme::Runtime> runtime,std::shared_ptr<parallelme::Program> program,std::shared_ptr<Coach> coach){

    auto labelsBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*(this->getDownLimit()-this->getUpLimit())*(this->getRightLimit()-this->getLeftLimit()));
    labelsBuffer->setSource(this->getLabels());
    auto widthBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*));
    unsigned int width = (this->getRightLimit()-this->getLeftLimit());
    widthBuffer->setSource(&width);
    auto ccountBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*(this->getDownLimit()-this->getUpLimit()));
    unsigned int *ccount = (unsigned int*) malloc(sizeof(unsigned int*)*(this->getDownLimit()-this->getUpLimit()));
    ccountBuffer->setSource(ccount);


    auto dataBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int)*coach->_dataSize);
    dataBuffer->setSource(coach->_data);
    auto dataSizeBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int));
    dataSizeBuffer->setSource(&coach->_dataSize);
    unsigned int result = 0;
    auto letterResultBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int));
    letterResultBuffer->setSource(&result);
    unsigned int *rotule = (unsigned int*) malloc(sizeof(unsigned int)*coach->_dataSize);
    auto rotuleBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int)*coach->_dataSize);
    rotuleBuffer->setSource(rotule);
    auto task = std::make_unique<parallelme::Task>(program);

    task->addKernel("crossing");
    task->addKernel("identification");
    task->setConfigFunction([=] (parallelme::DevicePtr &device, parallelme::KernelHash &kernelHash) {
            device = device;
            kernelHash["crossing"]
                ->setArg(0, labelsBuffer)
                ->setArg(1, widthBuffer)
                ->setArg(2, ccountBuffer)
                ->setWorkSize((this->getDownLimit()-this->getUpLimit()));
            kernelHash["identification"]
                ->setArg(0, dataBuffer)
                ->setArg(1, dataSizeBuffer)
                ->setArg(2, ccountBuffer)
                ->setArg(3, widthBuffer)
                ->setArg(4, rotuleBuffer)
                ->setArg(5, letterResultBuffer)
                ->setWorkSize(1); //code is sequential
    });
    runtime->submitTask(std::move(task));
    runtime->finish();
    ccountBuffer->copyTo(ccount);
    rotuleBuffer->copyTo(rotule);
    letterResultBuffer->copyTo(&result);
    __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%d ",result);
    /*for(unsigned int i=0;i<coach->_dataSize;i++){
        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%d ",rotule[i]);
    }*/



    this->_crossingRotuleSize = 0;
    for(unsigned int i=0; i<(this->getDownLimit()-this->getUpLimit()-1);i++){
        if(ccount[(i+1)] != ccount[i]){
                this->_crossingRotuleSize++;
        }
    }
    std::string stringRotule = "";
    this->_crossingRotule = (unsigned int*) malloc(sizeof(unsigned int*)*this->_crossingRotuleSize);
    int contLetters = 0;
    for(unsigned int i=0; i<(this->getDownLimit()-this->getUpLimit()-1);i++){
        if(ccount[(i+1)] != ccount[i]){
            stringRotule = stringRotule+" "+std::to_string(ccount[i]);
            this->_crossingRotule[contLetters] = ccount[i];
            contLetters++;
        }
    }


    /*
     ______________________________________
    | implements here the recognition phase|
     --------------------------------------
    */
    unsigned int pos = coach->_file.find(stringRotule);
    if(pos!=std::string::npos){
        if(!isInteger(coach->_file.substr((pos-3),1))){
            this->_letter = coach->_file.substr((pos-3),1);
            __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%s",coach->_file.substr((pos-3),1).c_str());
        }else{
            this->_letter = "";
        }
    }else{
        this->_letter = "";
    }


    //__android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%s - %s",stringRotule.c_str(),coach->_file.c_str());


    bool train = false;

    if(train){
        /*
            The training will to be stored in sdcard/traindata.txt
        */
        char trainchar[2] = "Q";
        FILE *trainfile = fopen("sdcard/traindata2.txt","a");
        fprintf(trainfile,"%s ",trainchar);
        for(int i=0;i<contLetters;i++){
            fprintf(trainfile,"%d ",this->_crossingRotule[i]);
        }
        fprintf(trainfile,"\n");
        fclose(trainfile);
    }


}