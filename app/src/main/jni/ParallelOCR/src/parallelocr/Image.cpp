//
// Created by rcarvs
//


#include <parallelocr/Image.hpp>
#include <android/log.h>

using namespace parallelocr;

Image::Image(JNIEnv* env,jobject *bitmap): _env(env),_bitmap(bitmap){
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
/*
|---------------------------------------------------------------|
|   ACHO QUE CONSIGO PARALELIZAR ESSA FUNÇÃO                    |
|_______________________________________________________________|
*/

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

void Image::toLabel(){
    
}






