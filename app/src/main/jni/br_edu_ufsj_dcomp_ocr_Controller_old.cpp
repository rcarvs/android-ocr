#include <jni.h>
#include <android/log.h>
#include <parallelme/ParallelME.hpp>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include "br_edu_ufsj_dcomp_ocr_Controller.h"

using namespace parallelme;

const static char gKernels[] =     
"__kernel void blackandwhite(__global uchar4 *image) {      \n"
"    int gid = get_global_id(0);                            \n"
"    uchar4 pixel = image[gid];                             \n"
"    if((pixel.x+pixel.y+pixel.z) > 645){                   \n"
"       pixel.x = pixel.y = pixel.z = 255;                  \n"    
"    }else{                                                 \n"    
"       pixel.x = pixel.y = pixel.z = 0;                    \n"    
"    }                                                      \n"    
"    image[gid] = pixel;                                    \n"
"}                                                          \n";


struct NativeData{
    std::shared_ptr<Runtime> runtime;
    std::shared_ptr<Program> program;
};


class JniBitmap{
    public:
    uint32_t* _storedBitmapPixels;
    AndroidBitmapInfo _bitmapInfo;
    JniBitmap(){
        _storedBitmapPixels = NULL;
    }
};

typedef struct{
    uint8_t alpha, red, green, blue;
} ARGB;





int32_t convertArgbToInt(ARGB argb)
{
    return (argb.alpha) | (argb.red << 24) | (argb.green << 16)
    | (argb.blue << 8);
}

void convertIntToArgb(uint32_t pixel, ARGB* argb)
{
    argb->red = ((pixel >> 24) & 0xff);
    argb->green = ((pixel >> 16) & 0xff);
    argb->blue = ((pixel >> 8) & 0xff);
    argb->alpha = (pixel & 0xff);
}

JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeCreateImageLabels(JNIEnv *env,jobject self,jobject handle){
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    AndroidBitmapInfo bitmapInfo;
    int ret;
    if ((ret = AndroidBitmap_getInfo(env, handle, &bitmapInfo)) < 0){        
        return;
    }

    if (jniBitmap->_storedBitmapPixels == NULL){        
        return;
    }
    uint32_t* pixels = jniBitmap->_storedBitmapPixels;
    ARGB argb;
    //ARGB argbComp;
    unsigned int* labels = (unsigned int*) malloc(sizeof(unsigned int)*(bitmapInfo.height*bitmapInfo.width));


    unsigned int label = 0;
    int size = (bitmapInfo.height*bitmapInfo.width);
    int i = 0;
    



    for(;i<size;i++){             
        convertIntToArgb(pixels[i],&argb);  
        label++; 

        labels[i] = argb.red+argb.green+argb.blue;      
        /*if((argb.red+argb.green+argb.blue) > 400){
            //is white
            labels[i] = 0;            
        }else{            
            int top  = (((int)(i-bitmapInfo.width) >= 0)?(i-bitmapInfo.width):-1);
            int topLeft = (((int)(i-bitmapInfo.width-1)>=0 && ((i-bitmapInfo.width)/bitmapInfo.width) == ((i-bitmapInfo.width-1)/bitmapInfo.width))?(i-bitmapInfo.width-1):-1);
            int topRight = (((int)(i-bitmapInfo.width+1)<size && ((i+bitmapInfo.width)/bitmapInfo.width) == ((i+bitmapInfo.width+1)/bitmapInfo.width))?(i-bitmapInfo.width+1):-1);
            int left = (((int)(i-1) >= 0 && (i/bitmapInfo.width)==((i-1)/bitmapInfo.width))?(i-1):-1);
            int right = (((int)(i+1) < size && (i/bitmapInfo.width)==((i+1)/bitmapInfo.width))?(i+1):-1);
            int down = (((int)(i+bitmapInfo.width) < size)?(i+bitmapInfo.width):-1);
            int downLeft = (((int)(i+bitmapInfo.width-1) < size && ((i+bitmapInfo.width-1)/bitmapInfo.width) == ((i+bitmapInfo.width)/bitmapInfo.width))?(i+bitmapInfo.width-1):-1);
            int downRight = (((int)(i+bitmapInfo.width+1) < size && ((i+bitmapInfo.width+1)/bitmapInfo.width) == ((i+bitmapInfo.width)/bitmapInfo.width))?(i+bitmapInfo.width+1):-1);            
            unsigned int equals[8];
            int contEquals = 0;
            if(top != -1){
                convertIntToArgb(pixels[top],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[top];
                    contEquals++;
                }
            }
            if(topLeft != -1){
                convertIntToArgb(pixels[topLeft],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[topLeft];
                    contEquals++;
                }
            }
            if(topRight != -1){
                convertIntToArgb(pixels[topRight],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[topRight];
                    contEquals++;
                }
            }
            if(left != -1){
                convertIntToArgb(pixels[left],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[left];
                    contEquals++;
                }
            }
            if(right != -1){
                convertIntToArgb(pixels[right],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[right];
                    contEquals++;
                }
            }
            if(down != -1){
                convertIntToArgb(pixels[down],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[down];
                    contEquals++;
                }
            }
            if(downLeft != -1){
                convertIntToArgb(pixels[downLeft],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[downLeft];
                    contEquals++;
                }
            }
            if(downRight != -1){
                convertIntToArgb(pixels[downRight],&argbComp);
                if((argb.red==argbComp.red) && (argb.green==argbComp.green) && (argb.blue==argbComp.blue)){
                    equals[contEquals] = labels[downRight];
                    contEquals++;
                }
            }            
            if(contEquals == 0){
                label++;
                labels[i] = label;                
            }else{                
                unsigned int downLabel = 0;
                for(int j=0;j<contEquals;j++){                
                    if(downLabel == 0){
                        downLabel = equals[j];
                    }else if(downLabel > equals[j]){
                        downLabel = equals[j];
                    }                                        
                }

                if(downLabel == 0){
                    label++;
                    labels[i] = label;                                    
                }else{
                    labels[i] = downLabel;
                }                              

            }

        }   
        */     
    }    

    FILE * arquivo = fopen("/sdcard/out.txt","w+");
    for(int i=0;i<(int) bitmapInfo.height;i++){
        for(int j=0;j<(int)bitmapInfo.width;j++){
            fprintf(arquivo, "%d ", labels[(bitmapInfo.width*i+j)]);    
        }
        fprintf(arquivo, "\n");    
    }

    fclose(arquivo);


}



JNIEXPORT jlong JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeInit(JNIEnv *env, jobject self) {
    JavaVM *jvm; //cria um ponteiro para uma jvm
    env->GetJavaVM(&jvm); //adiciona a jvm da execução para o ponteiro criado anteriormente
    if(!jvm) return (jlong) nullptr; //se n encontrar a jvm retorna um nullptr

    auto dataPointer = new NativeData(); //cria um ponteiro para os dados da struct NativeData
    dataPointer->runtime = std::make_shared<Runtime>(jvm,std::make_shared<SchedulerFCFS>()); //preenche o objeto runtime da struct nativedata
    dataPointer->program = std::make_shared<Program>(dataPointer->runtime, gKernels);

    return (jlong) dataPointer;
}
JNIEXPORT jobject JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeSetImage(JNIEnv *env,jobject self,jobject bitmap){

    AndroidBitmapInfo bitmapInfo;
    uint32_t* storedBitmapPixels = NULL;
    //LOGD("reading bitmap info...");
    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0){
       return NULL;
   }
    //LOGD("width:%d height:%d stride:%d", bitmapInfo.width, bitmapInfo.height, bitmapInfo.stride);
   if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888){

       return NULL;
   }
    //
    //read pixels of bitmap into native memory :
    //
    //LOGD("reading bitmap pixels...");
   void* bitmapPixels;
   if ((ret = AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0)
   {

       return NULL;
   }
   uint32_t* src = (uint32_t*) bitmapPixels;
   storedBitmapPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
   int pixelsCount = bitmapInfo.height * bitmapInfo.width;
   memcpy(storedBitmapPixels, src, sizeof(uint32_t) * pixelsCount);
   AndroidBitmap_unlockPixels(env, bitmap);
   JniBitmap *jniBitmap = new JniBitmap();
   jniBitmap->_bitmapInfo = bitmapInfo;
   jniBitmap->_storedBitmapPixels = storedBitmapPixels;
   return env->NewDirectByteBuffer(jniBitmap, 0);

}
JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeImageToBlackAndWhite(JNIEnv *env, jobject self,jlong dataPointerLong,jobject bitmap){
    auto dataPointer = (NativeData *) dataPointerLong;
    AndroidBitmapInfo bitmapInfo;    
    int ret;
    if((ret = AndroidBitmap_getInfo(env,bitmap,&bitmapInfo)) < 0){
        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Image error: %d",ret);
        return;
    }
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888){
        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Image Format Error");
        return;
    }

    

    long imageSize = bitmapInfo.height*bitmapInfo.width;
    auto bitmapBuffer = std::make_shared<Buffer>(Buffer::sizeGenerator(imageSize, Buffer::RGBA));
    bitmapBuffer->setAndroidBitmapSource(env,bitmap);
    auto task = std::make_unique<Task>(dataPointer->program);    
    task->addKernel("blackandwhite");
    task->setConfigFunction([=] (DevicePtr &device, KernelHash &kernelHash) {
        kernelHash["blackandwhite"]->setArg(0, bitmapBuffer)->setWorkSize(imageSize);
    });
    dataPointer->runtime->submitTask(std::move(task));
    dataPointer->runtime->finish();    

    bitmapBuffer->copyToAndroidBitmap(env, bitmap);    

}



JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeFreeImage(JNIEnv *env,jobject self,jobject handle){
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
       return;
   delete[] jniBitmap->_storedBitmapPixels;
   jniBitmap->_storedBitmapPixels = NULL;
   delete jniBitmap;
}



