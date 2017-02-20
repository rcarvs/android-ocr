//
// Created by Renan Carvalho
//


#include <parallelocr/Image.hpp>
#include <android/log.h>


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
}

void Image::bitmapTransformBlackAndWhite(){
    auto bitmapBuffer = std::make_shared<parallelme::Buffer>(parallelme::Buffer::sizeGenerator((this->getHeight()*this->getWidth()),parallelme::Buffer::RGBA));
    bitmapBuffer->setAndroidBitmapSource(this->getEnv(),this->getBitmap());
    auto task = std::make_unique<parallelme::Task>(this->getProgram());
    task->addKernel("blackandwhite");
    task->setConfigFunction([=] (parallelme::DevicePtr &device, parallelme::KernelHash &kernelHash) {
            device = device;
            kernelHash["blackandwhite"]->setArg(0, bitmapBuffer)->setWorkSize((this->getWidth()*this->getHeight()));
    });
    this->getRuntime()->submitTask(std::move(task));
    this->getRuntime()->finish();
    bitmapBuffer->copyToAndroidBitmap(this->getEnv(),this->getBitmap());
}

void Image::extractPixelsRGB(){
    uint32_t* line;
    this->setPixels((Pixel*) malloc(sizeof(Pixel)*(this->getHeight()*this->getWidth())));
    for(unsigned int y=0;y<this->getHeight();y++){
        line = (uint32_t*) this->getStoredPixels();
        for(unsigned int x=0;x<this->getWidth();x++){
            RGB rgb(line[x]);
            Pixel pixel(rgb);
            this->_pixels[(y*this->getHeight())+x] = pixel;
        }
        this->setStoredPixels((char*)this->getStoredPixels() + this->getStride());
    }
}


unsigned int Image::createDumblyLabels(){
    int upLabel = 0;
    for(unsigned int i=0;i<(this->getHeight()*this->getWidth());i++){
        //if is not white
        if((this->_pixels[i].getRGB().getRed()+this->_pixels[i].getRGB().getGreen()+this->_pixels[i].getRGB().getBlue()) != (255*3)){
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
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[top].getRGB().getRed()) &&
                 (this->_pixels[i].getRGB().getGreen()==this->_pixels[top].getRGB().getGreen()) &&
                 (this->_pixels[i].getRGB().getBlue()==this->_pixels[top].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[top].getLabel();
                    contEquals++;
                }
            }
            if(topLeft != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[topLeft].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[topLeft].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[topLeft].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[topLeft].getLabel();
                    contEquals++;
                }
            }
            if(topRight != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[topRight].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[topRight].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[topRight].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[topRight].getLabel();
                    contEquals++;
                }
            }
            if(left != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[left].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[left].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[left].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[left].getLabel();
                    contEquals++;
                }
            }
            if(right != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[right].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[right].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[right].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[right].getLabel();
                    contEquals++;
                }
            }
            if(down != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[down].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[down].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[down].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[down].getLabel();
                    contEquals++;
                }
            }
            if(downLeft != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[downLeft].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[downLeft].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[downLeft].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[downLeft].getLabel();
                    contEquals++;
                }
            }
            if(downRight != -1){
                if((this->_pixels[i].getRGB().getRed()==this->_pixels[downRight].getRGB().getRed()) &&
                (this->_pixels[i].getRGB().getGreen()==this->_pixels[downRight].getRGB().getGreen()) &&
                (this->_pixels[i].getRGB().getBlue()==this->_pixels[downRight].getRGB().getBlue())){
                    equals[contEquals] = this->_pixels[downRight].getLabel();
                    contEquals++;
                }
            }
            if(contEquals == 0){
                upLabel++;
                this->_pixels[i].setLabel(upLabel);
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
                    this->_pixels[i].setLabel(upLabel);
                }else{
                    this->_pixels[i].setLabel(downLabel);
                }
            }
        }
    }
    return upLabel;
}
void Image::checkLabel(unsigned int index){
    //just a check to now with thats not a wrong call
    if(this->_pixels[index].getLabel() != 0 && !this->_pixels[index].getChecked()){
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
            if(this->_pixels[neighbors[i]].getLabel() != 0 &&
                this->_pixels[neighbors[i]].getLabel() < this->_pixels[index].getLabel()){
                this->_pixels[index].setLabel(this->_pixels[neighbors[i]].getLabel());
            }
        }
        this->_pixels[index].setChecked(true);
        for(int i=0;i<count;i++){
            if(this->_pixels[neighbors[i]].getLabel() != 0 && !this->_pixels[neighbors[i]].getChecked()){
                checkLabel(neighbors[i]);
            }
        }
        for(int i=0;i<count;i++){
            if(this->_pixels[neighbors[i]].getLabel() != 0 &&
                this->_pixels[neighbors[i]].getLabel() < this->_pixels[index].getLabel()){
                this->_pixels[index].setLabel(this->_pixels[neighbors[i]].getLabel());
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
        if(this->_pixels[i].getLabel() != 0 && !this->_pixels[i].getChecked()){
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
                __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "%d %d %d %d.",this->_letters[this->getLetterCount()].getUpLimit(),this->_letters[this->getLetterCount()].getDownLimit(),this->_letters[this->getLetterCount()].getLeftLimit(),this->_letters[this->getLetterCount()].getRightLimit());

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
                        this->_pixels[
                                    (this->getWidth()*(this->_letters[this->getLetterCount()].getUpLimit()+y))+
                                    (this->_letters[this->getLetterCount()].getLeftLimit()+x)
                                    ].getLabel());

                    }
                }

                //now I have a letter in another vector and it's ready to be processed for feature extraction
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






