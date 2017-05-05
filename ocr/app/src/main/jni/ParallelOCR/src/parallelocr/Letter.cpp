//
// Created by Renan Carvalho
//
#include <parallelocr/Letter.hpp>
#include <android/log.h>
#include <string>
#include <time.h>

using namespace parallelocr;
using namespace parallelus;
inline bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

char _alfabhet[26] ={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','K','R','S','T','U','V','W','X','Y','Z'};
void Letter::crossing(std::shared_ptr<parallelus::Runtime> runtime,std::shared_ptr<parallelus::Program> program,std::shared_ptr<Coach> coach){
    clock_t begin = clock();
    auto labelsBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int)*(this->getDownLimit()-this->getUpLimit())*(this->getRightLimit()-this->getLeftLimit()));
    labelsBuffer->setSource(this->getLabels());
    auto widthBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int));
    unsigned int width = (this->getRightLimit()-this->getLeftLimit());
    widthBuffer->setSource(&width);
    auto ccountBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int)*(this->getDownLimit()-this->getUpLimit()));
    unsigned int *ccount = (unsigned int*) malloc(sizeof(unsigned int)*(this->getDownLimit()-this->getUpLimit()));
    ccountBuffer->setSource(ccount);


    auto dataBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int)*coach->_dataSize);
    dataBuffer->setSource(coach->_data);
    auto dataSizeBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int));
    dataSizeBuffer->setSource(&coach->_dataSize);

    auto heightBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int));
    unsigned int height = (this->getDownLimit()-this->getUpLimit());
    heightBuffer->setSource(&height);

    unsigned int result = 0;
    auto letterResultBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int));
    letterResultBuffer->setSource(&result);
    unsigned int *rotule = (unsigned int*) malloc(sizeof(unsigned int)*coach->_dataSize);
    auto rotuleBuffer = std::make_shared<parallelus::Buffer>(sizeof(unsigned int)*coach->_dataSize);
    rotuleBuffer->setSource(rotule);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_buffers[coach->_count_evaluation] = elapsed_secs;
    begin = clock();
    auto task = std::make_unique<parallelus::Task>(program);

    //task->addKernel("crossing");
    for(int i=1;i<=1;i++){
        std::string fname = "fe"+std::to_string(i);
        task->addKernel(fname);
    }
    task->addKernel("identification");
    task->setConfigFunction([=] (parallelus::DevicePtr &device, parallelus::KernelHash &kernelHash,unsigned type) {
            device = device;
            for(int i=1;i<=1;i++){
                std::string fname = "fe"+std::to_string(i);
                kernelHash[fname]
                ->setArg(0, labelsBuffer,type)
                ->setArg(1, widthBuffer,type)
                ->setArg(2, ccountBuffer,type)
                ->setWorkSize((this->getDownLimit()-this->getUpLimit()),1,1,type);
            }
            kernelHash["identification"]
                ->setArg(0, dataBuffer,type)
                ->setArg(1, dataSizeBuffer,type)
                ->setArg(2, ccountBuffer,type)
                ->setArg(3, heightBuffer,type)
                ->setArg(4, rotuleBuffer,type)
                ->setArg(5, letterResultBuffer,type)
                ->setWorkSize(1,1,1,type); //code is sequential
    });
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_tasks[coach->_count_evaluation] = elapsed_secs;
    begin = clock();
    runtime->submitTask(std::move(task),3);//execucao externa

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_submission[coach->_count_evaluation] = elapsed_secs;

    begin = clock();
    runtime->finish();
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    coach->_finish[coach->_count_evaluation] = elapsed_secs;

    /*task->setFinishFunction([=] (DevicePtr &device, KernelHash &kernelHash, unsigned type){
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui");
        //begin = clock();
        unsigned t = type;
        t=!t;
        kernelHash = kernelHash;
        device = device;

        ccountBuffer->copyTo(ccount,0);
        //letterResultBuffer->copyTo(&result,1);

        coach->_result[coach->_count_evaluation] = elapsed_secs;
        coach->_count_evaluation++;
        //end = clock();
        //elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        if((result-17) <= 26){
            __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui69 %d",(result));
            this->_letter = coach->_alfabhet[(result-17)];
            __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui69*2");
        }else{
            this->_letter = "";
        }
    });*/


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