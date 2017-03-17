//
// Created by Renan Carvalho
//
#include <parallelocr/Letter.hpp>
#include <android/log.h>
#include <string>

using namespace parallelocr;

inline bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

void Letter::crossing(std::shared_ptr<parallelme::Runtime> runtime,std::shared_ptr<parallelme::Program> program,std::shared_ptr<Coach> coach){

    auto labelsBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int)*(this->getDownLimit()-this->getUpLimit())*(this->getRightLimit()-this->getLeftLimit()));
    labelsBuffer->setSource(this->getLabels());
    auto widthBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int));
    unsigned int width = (this->getRightLimit()-this->getLeftLimit());
    widthBuffer->setSource(&width);
    auto ccountBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int)*(this->getDownLimit()-this->getUpLimit()));
    unsigned int *ccount = (unsigned int*) malloc(sizeof(unsigned int)*(this->getDownLimit()-this->getUpLimit()));
    ccountBuffer->setSource(ccount);


    auto dataBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int)*coach->_dataSize);
    dataBuffer->setSource(coach->_data);
    auto dataSizeBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int));
    dataSizeBuffer->setSource(&coach->_dataSize);

    auto heightBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int));
    unsigned int height = (this->getDownLimit()-this->getUpLimit());
    heightBuffer->setSource(&height);

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
                ->setArg(3, heightBuffer)
                ->setArg(4, rotuleBuffer)
                ->setArg(5, letterResultBuffer)
                ->setWorkSize(1); //code is sequential
    });
    runtime->submitTask(std::move(task));
    runtime->finish();
    ccountBuffer->copyTo(ccount);
    letterResultBuffer->copyTo(&result);
    if((result-17) <= 26){
        this->_letter = coach->_alfabhet[result-17];
    }else{
        this->_letter = "";
    }
    /*bool train = false;

    if(train){

        char trainchar[2] = "Q";
        FILE *trainfile = fopen("sdcard/traindata2.txt","a");
        fprintf(trainfile,"%s ",trainchar);
        for(int i=0;i<contLetters;i++){
            fprintf(trainfile,"%d ",this->_crossingRotule[i]);
        }
        fprintf(trainfile,"\n");
        fclose(trainfile);
    }*/


}