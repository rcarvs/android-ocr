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
    int countCross = 0;
    for(unsigned int i=0; i<(this->getDownLimit()-this->getUpLimit());i++){
        if(i == 0){
            countCross++;
        }else{
            if(ccount[(i-1)] != ccount[i]){
                countCross++;
            }
        }
    }
    this->_crossingRotule = (char*) malloc(sizeof(char*)*(countCross+1));
    int contLetters = 0;
    for(unsigned int i=0; i<(this->getDownLimit()-this->getUpLimit());i++){
        if(i == 0){
            std::snprintf(&this->_crossingRotule[contLetters],16,"%d",ccount[i]);
            contLetters++;
        }else{
            if(ccount[(i-1)] != ccount[i]){
                std::snprintf(&this->_crossingRotule[contLetters],16,"%d",ccount[i]);
                contLetters++;
            }
        }
    }

    this->_crossingRotule[contLetters] = '\0';
    free(ccount);

}