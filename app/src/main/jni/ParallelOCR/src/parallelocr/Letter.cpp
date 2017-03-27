//
// Created by Renan Carvalho
//
#include <parallelocr/Letter.hpp>
#include <android/log.h>
#include <string>
#include <time.h>

using namespace parallelocr;

inline bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

char _alfabhet[26] ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','K','R','S','T','U','V','W','X','Y','Z'};
void Letter::crossing(std::shared_ptr<parallelme::Runtime> runtime,std::shared_ptr<parallelme::Program> program,std::shared_ptr<Coach> coach){
    clock_t begin = clock();
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
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_buffers[coach->_count_evaluation] = elapsed_secs;
    begin = clock();
    auto task = std::make_unique<parallelme::Task>(program);

    task->addKernel("crossing");
    /*for(int i=1;i<=50;i++){
        std::string fname = "fe"+std::to_string(i);
        task->addKernel(fname);
    }*/
    task->addKernel("identification");
    task->setConfigFunction([=] (parallelme::DevicePtr &device, parallelme::KernelHash &kernelHash) {
            device = device;
            kernelHash["crossing"]
                ->setArg(0, labelsBuffer)
                ->setArg(1, widthBuffer)
                ->setArg(2, ccountBuffer)
                ->setWorkSize((this->getDownLimit()-this->getUpLimit()));
            /*for(int i=1;i<=100;i++){
                std::string fname = "fe"+std::to_string(i);
                kernelHash[fname]
                ->setArg(0, labelsBuffer)
                ->setArg(1, widthBuffer)
                ->setArg(2, ccountBuffer)
                ->setWorkSize((this->getDownLimit()-this->getUpLimit()));
            }*/

kernelHash["identification"]
                ->setArg(0, dataBuffer)
                ->setArg(1, dataSizeBuffer)
                ->setArg(2, ccountBuffer)
                ->setArg(3, heightBuffer)
                ->setArg(4, rotuleBuffer)
                ->setArg(5, letterResultBuffer)
                ->setWorkSize(1); //code is sequential
    });
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_tasks[coach->_count_evaluation] = elapsed_secs;
    begin = clock();
    runtime->submitTask(std::move(task));

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_submission[coach->_count_evaluation] = elapsed_secs;

    begin = clock();
    runtime->finish();
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_finish[coach->_count_evaluation] = elapsed_secs;
    begin = clock();
    ccountBuffer->copyTo(ccount);
    letterResultBuffer->copyTo(&result);
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_result[coach->_count_evaluation] = elapsed_secs;
    coach->_count_evaluation++;
    __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui");
    if((result-17) <= 26){
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui69 %d",(result));
        this->_letter = coach->_alfabhet[(result-17)];
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui69*2");
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