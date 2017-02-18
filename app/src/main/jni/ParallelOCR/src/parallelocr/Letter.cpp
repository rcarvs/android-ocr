//
// Created by rcarvs
//
#include <parallelocr/Letter.hpp>
#include <android/log.h>
#include <cstring>
using namespace parallelocr;

void Letter::crossing(std::shared_ptr<parallelme::Runtime> runtime,std::shared_ptr<parallelme::Program> program){

    auto labelsBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*(this->getDownLimit()-this->getUpLimit())*(this->getRightLimit()-this->getLeftLimit()));
    labelsBuffer->setSource(this->getLabels());
    auto widthBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*));
    unsigned int width = (this->getRightLimit()-this->getLeftLimit());
    widthBuffer->setSource(&width);
    auto ccountBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*(this->getDownLimit()-this->getUpLimit()));
    unsigned int *ccount = (unsigned int*) malloc(sizeof(unsigned int*)*(this->getDownLimit()-this->getUpLimit()));
    ccountBuffer->setSource(ccount);

    auto task = std::make_unique<parallelme::Task>(program);

    task->addKernel("crossing");
    task->setConfigFunction([=] (parallelme::DevicePtr &device, parallelme::KernelHash &kernelHash) {
            device = device;
            kernelHash["crossing"]
                ->setArg(0, labelsBuffer)
                ->setArg(1, widthBuffer)
                ->setArg(2, ccountBuffer)
                ->setWorkSize((this->getDownLimit()-this->getUpLimit()));
    });
    runtime->submitTask(std::move(task));
    runtime->finish();

    ccountBuffer->copyTo(ccount);
    for(unsigned int i=0; i<(this->getDownLimit()-this->getUpLimit());i++){
        if(ccount[(i+1)] != ccount[i]){
                this->_crossingRotuleSize++;
        }
    }
    this->_crossingRotule = (unsigned int*) malloc(sizeof(unsigned int*)*this->_crossingRotuleSize);
    int contLetters = 0;
    for(unsigned int i=0; i<(this->getDownLimit()-this->getUpLimit()-1);i++){
        if(ccount[(i+1)] != ccount[i]){
            this->_crossingRotule[contLetters] = ccount[i];
            contLetters++;
        }
    }

    for(int i=0;i<contLetters;i++){
        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%d", this->_crossingRotule[i]);
    }



    free(ccount);

}