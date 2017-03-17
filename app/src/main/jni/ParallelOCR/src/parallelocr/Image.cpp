//
// Created by Renan Carvalho
//


#include <parallelocr/Image.hpp>
#include <android/log.h>
#include <time.h>

using namespace parallelocr;

Image::Image(JNIEnv* env,jobject *bitmap): _env(env),_bitmap(bitmap),_letterCount(0){
    AndroidBitmapInfo  info;
    int ret;

    if ((ret = AndroidBitmap_getInfo(env, *bitmap, &info)) < 0 ||
    info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 ||
    (ret = AndroidBitmap_lockPixels(env, *bitmap, &this->_storedPixels)) < 0) {
        return;
    }
    this->setStride(info.stride);
    this->setWidth(info.width);
    this->setHeight(info.height);

    //this is for parallelize of pixels info extraction
    this->_r = (unsigned int*) malloc(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    this->_g = (unsigned int*) malloc(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    this->_b = (unsigned int*) malloc(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    this->_label = (unsigned int*) malloc(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    this->_checked = (unsigned int*) malloc(sizeof(unsigned int*)*this->getWidth()*this->getHeight());

}

void Image::bitmapTransformBlackAndWhite(){
    auto bitmapBuffer = std::make_shared<parallelme::Buffer>(parallelme::Buffer::sizeGenerator((this->getHeight()*this->getWidth()),parallelme::Buffer::RGBA));
    bitmapBuffer->setAndroidBitmapSource(this->getEnv(),this->getBitmap());
    auto rBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    rBuffer->setSource(this->_r);
    auto gBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    gBuffer->setSource(this->_g);
    auto bBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    bBuffer->setSource(this->_b);
    auto labelBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    labelBuffer->setSource(this->_label);
    auto checkedBuffer = std::make_shared<parallelme::Buffer>(sizeof(unsigned int*)*this->getWidth()*this->getHeight());
    checkedBuffer->setSource(this->_checked);

    auto task = std::make_unique<parallelme::Task>(this->getProgram());
    task->addKernel("blackandwhite");
    task->setConfigFunction([=] (parallelme::DevicePtr &device, parallelme::KernelHash &kernelHash) {
            device = device;
            kernelHash["blackandwhite"]
            ->setArg(0, bitmapBuffer)
            ->setArg(1, rBuffer)
            ->setArg(2, gBuffer)
            ->setArg(3, bBuffer)
            ->setArg(4, labelBuffer)
            ->setArg(5, checkedBuffer)
            ->setWorkSize((this->getWidth()*this->getHeight()));
    });
    clock_t begin = clock();
    this->getRuntime()->submitTask(std::move(task));
    this->getRuntime()->finish();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp Time", "%f ", time_spent);
    bitmapBuffer->copyToAndroidBitmap(this->getEnv(),this->getBitmap());
    rBuffer->copyTo(this->_r);
    gBuffer->copyTo(this->_g);
    bBuffer->copyTo(this->_b);
    labelBuffer->copyTo(this->_label);
    checkedBuffer->copyTo(this->_checked);


}

unsigned int Image::createDumblyLabels(){
    int upLabel = 0;
    for(unsigned int i=0;i<(this->getHeight()*this->getWidth());i++){
        //if is not white
        if((this->_r[i]+this->_g[i]+this->_b[i]) != (255*3)){
            //get the eight neighbors of actual pixel
            int top  = (((int)(i-this->getWidth()) >= 0)?(i-this->getWidth()):-1);
            int topLeft = (((int)(i-this->getWidth()-1)>=0 && ((i-this->getWidth())/this->getWidth()) == ((i-this->getWidth()-1)/this->getWidth()))?(i-this->getWidth()-1):-1);
            int topRight = (((unsigned int)(i-this->getWidth()+1)<(this->getWidth()*this->getHeight()) && ((i+this->getWidth())/this->getWidth()) == ((i+this->getWidth()+1)/this->getWidth()))?(i-this->getWidth()+1):-1);
            int left = (((int)(i-1) >= 0 && (i/this->getWidth())==((i-1)/this->getWidth()))?(i-1):-1);
            int right = (((i+1) < (this->getWidth()*this->getHeight()) && (i/this->getWidth())==((i+1)/this->getWidth()))?(i+1):-1);
            int down = (((i+this->getWidth()) < (this->getWidth()*this->getHeight()))?(i+this->getWidth()):-1);
            int downLeft = (((i+this->getWidth()-1) < (this->getWidth()*this->getHeight()) && ((i+this->getWidth()-1)/this->getWidth()) == ((i+this->getWidth())/this->getWidth()))?(i+this->getWidth()-1):-1);
            int downRight = (((i+this->getWidth()+1) < (this->getWidth()*this->getHeight()) && ((i+this->getWidth()+1)/this->getWidth()) == ((i+this->getWidth())/this->getWidth()))?(i+this->getWidth()+1):-1);

            unsigned int equals[8];
            int contEquals = 0;
            //now compare each neighbor to now if is equal
            if(top != -1){
                if((this->_r[i]==this->_r[top]) &&
                 (this->_g[i]==this->_g[top]) &&
                 (this->_b[i]==this->_b[top])){
                    equals[contEquals] = this->_label[top];
                    contEquals++;
                }
            }
            if(topLeft != -1){
                if((this->_r[i]==this->_r[topLeft]) &&
                (this->_g[i]==this->_g[topLeft]) &&
                (this->_b[i]==this->_b[topLeft])){
                    equals[contEquals] = this->_label[topLeft];
                    contEquals++;
                }
            }
            if(topRight != -1){
                if((this->_r[i]==this->_r[topRight]) &&
                (this->_g[i]==this->_g[topRight]) &&
                (this->_b[i]==this->_b[topRight])){
                    equals[contEquals] = this->_label[topRight];
                    contEquals++;
                }
            }
            if(left != -1){
                if((this->_r[i]==this->_r[left]) &&
                (this->_g[i]==this->_g[left]) &&
                (this->_b[i]==this->_b[left])){
                    equals[contEquals] = this->_label[left];
                    contEquals++;
                }
            }
            if(right != -1){
                if((this->_r[i]==this->_r[right]) &&
                (this->_g[i]==this->_g[right]) &&
                (this->_b[i]==this->_b[right])){
                    equals[contEquals] = this->_label[right];
                    contEquals++;
                }
            }
            if(down != -1){
                if((this->_r[i]==this->_r[down]) &&
                (this->_g[i]==this->_g[down]) &&
                (this->_b[i]==this->_b[down])){
                    equals[contEquals] = this->_label[down];
                    contEquals++;
                }
            }
            if(downLeft != -1){
                if((this->_r[i]==this->_r[downLeft]) &&
                (this->_g[i]==this->_g[downLeft]) &&
                (this->_b[i]==this->_b[downLeft])){
                    equals[contEquals] = this->_label[downLeft];
                    contEquals++;
                }
            }
            if(downRight != -1){
                if((this->_r[i]==this->_r[downRight]) &&
                (this->_g[i]==this->_g[downRight]) &&
                (this->_b[i]==this->_b[downRight])){
                    equals[contEquals] = this->_label[downRight];
                    contEquals++;
                }
            }
            if(contEquals == 0){
                upLabel++;
                this->_label[i] = upLabel;
            }else{
                int downLabel = 0;
                for(int j=0;j<contEquals;j++){
                    if(equals[j] != 0){
                        if(downLabel == 0){
                            downLabel = equals[j];
                        }else if(downLabel >(int) equals[j]){
                            downLabel = equals[j];
                        }
                    }
                }
                if(downLabel == 0){
                    upLabel++;
                    this->_label[i] = upLabel;
                }else{
                    this->_label[i] = downLabel;
                }
            }
        }
    }
    return upLabel;
}
void Image::checkLabel(unsigned int index){
    //just a check to now with thats not a wrong call
    if(this->_label[index] != 0 && this->_checked[index] == 0){
        //get the neighbors not checked yet and put in a list to be executed
        int *neighbors = (int*) malloc(sizeof(int)*8);
        int count = 0;
        (((int)(index-this->getWidth()) >= 0 //check if up a line still have matrix
        )?(neighbors[count] = (index-this->getWidth()),count++):-1);

        (((int)(index-this->getWidth()-1)>=0 && //check if subtract a line and one element still have matrix
        ((index-this->getWidth())/this->getWidth()) == ((index-this->getWidth()-1)/this->getWidth())  //check if the elements is in the same line
        )?(neighbors[count] = (index-this->getWidth()-1),count++):-1);

        (((unsigned int)(index-this->getWidth()+1)<(this->getWidth()*this->getHeight()) && //check if add a line and one element still have matrix
        ((index+this->getWidth())/this->getWidth()) == ((index+this->getWidth()+1)/this->getWidth()) //check if the elements is in the same line of matrix
        )?(neighbors[count] = (index-this->getWidth()+1),count++):-1);

        (((int)(index-1) >= 0 && //still have matrix?
        (index/this->getWidth())==((index-1)/this->getWidth()) //is the same line?
        )?(neighbors[count] = (index-1),count++):-1);

        (((index+1) < (this->getWidth()*this->getHeight()) && //still have matrix?
        (index/this->getWidth())==((index+1)/this->getWidth())  //is the same line?
        )?(neighbors[count] = (index+1),count++):-1);

        (((index+this->getWidth()) < (this->getWidth()*this->getHeight())  //still have matrix?
        )?(neighbors[count] = (index+this->getWidth()),count++):-1);

        (((index+this->getWidth()-1) < (this->getWidth()*this->getHeight()) && //still have matrix
        ((index+this->getWidth()-1)/this->getWidth()) == ((index+this->getWidth())/this->getWidth())  //is the same line?
        )?(neighbors[count] = (index+this->getWidth()-1),count++):-1);

        (((index+this->getWidth()+1) < (this->getWidth()*this->getHeight()) && //still have matrix
        ((index+this->getWidth()+1)/this->getWidth()) == ((index+this->getWidth())/this->getWidth())  //is the same line?
        )?(neighbors[count] = (index+this->getWidth()+1),count++):-1);

        for(int i=0;i<count;i++){
            if(this->_label[neighbors[i]] != 0 &&
                this->_label[neighbors[i]] < this->_label[index]){
                this->_label[index] = this->_label[neighbors[i]];
            }
        }
        this->_checked[index] = 1;
        for(int i=0;i<count;i++){
            if(this->_label[neighbors[i]] != 0 && this->_checked[neighbors[i]] == 0){
                checkLabel(neighbors[i]);
            }
        }
        for(int i=0;i<count;i++){
            if(this->_label[neighbors[i]] != 0 &&
                this->_label[neighbors[i]] < this->_label[index]){
                this->_label[index] = this->_label[neighbors[i]];
            }
        }
        if((unsigned int)((index+1)/this->getWidth()) < this->_letters[this->_letterCount].getUpLimit()){
            this->_letters[this->_letterCount].setUpLimit((unsigned int)((index+1)/this->getWidth()));
        }
        if((unsigned int)((index+1)/this->getWidth()) > this->_letters[this->_letterCount].getDownLimit()){
            this->_letters[this->_letterCount].setDownLimit((unsigned int)((index+1)/this->getWidth()));
        }
        if((index%this->getWidth()) < (unsigned int)this->_letters[this->_letterCount].getLeftLimit()){
            this->_letters[this->_letterCount].setLeftLimit((unsigned int)(index%this->getWidth()));
        }
        if((index%this->getWidth()) > (unsigned int)this->_letters[this->_letterCount].getRightLimit()){
            this->_letters[this->_letterCount].setRightLimit((unsigned int)(index%this->getWidth()));
        }

        free(neighbors);
    }
}

void Image::relabelAndSearchLetters(unsigned int uplabel){
    //locate the max quantity of letters in first labeling
    this->setLetters((Letter*) malloc(sizeof(Letter)*uplabel));
    
    for(unsigned int i = 0;i<(this->getWidth()*this->getHeight());i++){
        //enter if the pixel is not white and not checked yet
        if(this->_label[i] != 0 && this->_checked[i] == 0){
            //first call for check label
            //start letter with your inverse
            this->_letters[this->getLetterCount()].setUpLimit(this->getHeight());
            this->_letters[this->getLetterCount()].setDownLimit(0);
            this->_letters[this->getLetterCount()].setLeftLimit(this->getWidth());
            this->_letters[this->getLetterCount()].setRightLimit(0);


            checkLabel(i);

            /*
            ------------------------------------------------------------
            |   Aqui vira um submit task para rodar a identificação    |
            ____________________________________________________________
            */
            if(this->_letters[this->getLetterCount()].getDownLimit() != this->_letters[this->getLetterCount()].getUpLimit() && this->_letters[this->getLetterCount()].getRightLimit() != this->_letters[this->getLetterCount()].getLeftLimit()){
                //__android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%d %d %d %d.",this->_letters[this->getLetterCount()].getUpLimit(),this->_letters[this->getLetterCount()].getDownLimit(),this->_letters[this->getLetterCount()].getLeftLimit(),this->_letters[this->getLetterCount()].getRightLimit());

                //here I have the letters limits. Create a copy of matrix for send to buffer memory
                //it can go to another thread

                this->_letters[this->getLetterCount()].setLabels((unsigned  int*) malloc(sizeof(unsigned int)*(this->_letters[this->getLetterCount()].getDownLimit()-this->_letters[this->getLetterCount()].getUpLimit())*(this->_letters[this->getLetterCount()].getRightLimit()-this->_letters[this->getLetterCount()].getLeftLimit())));

                for(unsigned int y = 0;
                    y < (this->_letters[this->getLetterCount()].getDownLimit()-this->_letters[this->getLetterCount()].getUpLimit());
                    y++){
                    for (unsigned int x = 0;
                        x < (this->_letters[this->getLetterCount()].getRightLimit()-this->_letters[this->getLetterCount()].getLeftLimit());
                        x++){

                        /*
                        DEBUGS
                        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Top: %d", this->_letters[this->getLetterCount()].getUpLimit()+y);
                        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Left: %d", this->_letters[this->getLetterCount()].getLeftLimit()+x);
                        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Pos: %d - Label: %d", (y*(this->_letters[this->getLetterCount()].getRightLimit()-this->_letters[this->getLetterCount()].getLeftLimit())+x),this->_pixels[(this->getWidth()*(this->_letters[this->getLetterCount()].getUpLimit()+y))+(this->_letters[this->getLetterCount()].getLeftLimit()+x)].getLabel());
                        */
                        this->_letters[this->getLetterCount()].setLabelElement(
                        (y*(this->_letters[this->getLetterCount()].getRightLimit()-this->_letters[this->getLetterCount()].getLeftLimit())+x),
                        this->_label[(this->getWidth()*(this->_letters[this->getLetterCount()].getUpLimit()+y))
                                    +(this->_letters[this->getLetterCount()].getLeftLimit()+x)]);
                    }
                }
                //now I have a letter in another vector and it's ready to be processed for feature extraction
                //create the task in the crossing function
                this->_letters[this->getLetterCount()].crossing(this->getRuntime(),this->getProgram(),this->getCoach());


            }
            /*
            ----------------------------------------
            |    ATÉ AQUI NA NOVA THREAD           |
            ----------------------------------------
            */


            this->_letterCount++;

        }
    }
}
void Image::toLabel(){
    //first label dumbly each pixel with toLabelCreateLabels
    unsigned int uplabel = this->createDumblyLabels();
    //second, resolve the conflicts AND cut the letter
    relabelAndSearchLetters(uplabel);

}






