#include <jni.h>
#include <android/log.h>
#include <parallelme/ParallelME.hpp>
#include <parallelocr/ParallelOCR.hpp>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include "br_edu_ufsj_dcomp_ocr_Controller.h"

using namespace parallelme;
using namespace parallelocr;



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



typedef struct{
    uint8_t red, green, blue;
} RGB;

void convertIntToRgb(uint32_t pixel, RGB* rgb){
    rgb->red = (int) ((pixel & 0x00FF0000) >> 16);
    rgb->green = (int)((pixel & 0x0000FF00) >> 8);
    rgb->blue = (int) (pixel & 0x00000FF);
}

typedef struct{
    int left, top, right, bottom;    
    char letter;
} letter;




JNIEXPORT jlong JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeInit(JNIEnv *env, jobject self) {
    JavaVM *jvm; //cria um ponteiro para uma jvm
    env->GetJavaVM(&jvm); //adiciona a jvm da execução para o ponteiro criado anteriormente
    if(!jvm) return (jlong) nullptr; //se n encontrar a jvm retorna um nullptr

    auto dataPointer = new NativeData(); //cria um ponteiro para os dados da struct NativeData
    dataPointer->runtime = std::make_shared<Runtime>(jvm,std::make_shared<SchedulerFCFS>()); //preenche o objeto runtime da struct nativedata
    dataPointer->program = std::make_shared<Program>(dataPointer->runtime, gKernels);

    return (jlong) dataPointer;
}



JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeCreateImageLabels(JNIEnv *env,jobject self,jlong dataPointerLong,jobject bitmap){

    parallelocr::Image image(bitmap);
    //first pass will put image in black and white
    nativeImageToBlackAndWhite(env,dataPointerLong,bitmap);
    //secont pass is access image pixels
    AndroidBitmapInfo  info;
    int ret;
    void* pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    

    uint32_t* line;
    //now, the pixels are in var pixels, pass that for a unidimencional vector
    int size = (info.height*info.width);
    RGB rgbPixels[size];
    int count = 0;
    for(unsigned int yy = 0; yy < info.height; yy++){
        line = (uint32_t*)pixels;
        for(unsigned int xx =0; xx < info.width; xx++){                    
            convertIntToRgb(line[xx],&rgbPixels[count]);
            count++;
        }

        pixels = (char*)pixels + info.stride;
    }

    int *labels = (int*) malloc(sizeof(int)*count);
    
    /*for(int i = 0;i<count;i++){
        labels[i] = 0;        
    }*/

    RGB rgbComp;
    int label = 0;
    for(int i=0;i<count;i++){
        if((rgbPixels[i].red+rgbPixels[i].green+rgbPixels[i].blue) != (255*3)){        
            int top  = (((int)(i-info.width) >= 0)?(i-info.width):-1);
            int topLeft = (((int)(i-info.width-1)>=0 && ((i-info.width)/info.width) == ((i-info.width-1)/info.width))?(i-info.width-1):-1);
            int topRight = (((int)(i-info.width+1)<size && ((i+info.width)/info.width) == ((i+info.width+1)/info.width))?(i-info.width+1):-1);
            int left = (((int)(i-1) >= 0 && (i/info.width)==((i-1)/info.width))?(i-1):-1);
            int right = (((int)(i+1) < size && (i/info.width)==((i+1)/info.width))?(i+1):-1);
            int down = (((int)(i+info.width) < size)?(i+info.width):-1);
            int downLeft = (((int)(i+info.width-1) < size && ((i+info.width-1)/info.width) == ((i+info.width)/info.width))?(i+info.width-1):-1);
            int downRight = (((int)(i+info.width+1) < size && ((i+info.width+1)/info.width) == ((i+info.width)/info.width))?(i+info.width+1):-1);        

            unsigned int equals[8];
            int contEquals = 0;            

            if(top != -1){
                rgbComp = rgbPixels[top];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[top];
                    contEquals++;
                }
            }
            if(topLeft != -1){
                rgbComp = rgbPixels[topLeft];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[topLeft];
                    contEquals++;
                }
            }
            if(topRight != -1){                
                rgbComp = rgbPixels[topRight];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[topRight];
                    contEquals++;
                }
            }
            if(left != -1){                
                rgbComp = rgbPixels[left];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[left];
                    contEquals++;
                }
            }
            if(right != -1){                
                rgbComp = rgbPixels[right];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[right];
                    contEquals++;
                }
            }
            if(down != -1){                
                rgbComp = rgbPixels[down];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[down];
                    contEquals++;
                }
            }
            if(downLeft != -1){                
                rgbComp = rgbPixels[downLeft];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[downLeft];
                    contEquals++;
                }
            }
            if(downRight != -1){                
                rgbComp = rgbPixels[downRight];
                if((rgbPixels[i].red==rgbComp.red) && (rgbPixels[i].green==rgbComp.green) && (rgbPixels[i].blue==rgbComp.blue)){
                    equals[contEquals] = labels[downRight];
                    contEquals++;
                }
            }              
            if(contEquals == 0){
                label++;
                labels[i] = label;                
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
                    label++;
                    labels[i] = label;                                    
                }else{                    
                    labels[i] = downLabel;
                }                                                    
            }


        }
    }
    //now, lets create a list for resolve equivalences
    int equivalences[(label+1)][label];
    for(int i=0;i<label;i++){
        for(int j=0;j<label;j++){
            equivalences[i][j] = 0;
        }
    }

    for(int i=0;i<=count;i++){
        if(labels[i] != 0){
            int top  = (((int)(i-info.width) >= 0)?labels[(i-info.width)]:0);
            int topLeft = (((int)(i-info.width-1)>=0 && ((i-info.width)/info.width) == ((i-info.width-1)/info.width))?labels[(i-info.width-1)]:0);
            int topRight = (((int)(i-info.width+1)<size && ((i+info.width)/info.width) == ((i+info.width+1)/info.width))?labels[(i-info.width+1)]:0);
            int left = (((int)(i-1) >= 0 && (i/info.width)==((i-1)/info.width))?labels[(i-1)]:0);
            int right = (((int)(i+1) < size && (i/info.width)==((i+1)/info.width))?labels[(i+1)]:0);
            int down = (((int)(i+info.width) < size)?labels[(i+info.width)]:0);
            int downLeft = (((int)(i+info.width-1) < size && ((i+info.width-1)/info.width) == ((i+info.width)/info.width))?labels[(i+info.width-1)]:0);
            int downRight = (((int)(i+info.width+1) < size && ((i+info.width+1)/info.width) == ((i+info.width)/info.width))?labels[(i+info.width+1)]:0);  

            int countEquivalences = 0;
            bool insert;
            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == labels[i]){
                    insert = false;
                    break;
                }
            }            
            if(insert){
                equivalences[labels[i]][countEquivalences] = labels[i];
                countEquivalences++;
            }


            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == top){
                    insert = false;
                    break;
                }
            }
            if(top != 0 && insert){
                equivalences[labels[i]][countEquivalences] = top;
                countEquivalences++;
            }

            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == topLeft){
                    insert = false;
                    break;
                }
            }
            if(topLeft != 0 && insert){
                equivalences[labels[i]][countEquivalences] = topLeft;
                countEquivalences++;
            }

            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == topRight){
                    insert = false;
                    break;
                }
            }
            if(topRight != 0 && insert){
                equivalences[labels[i]][countEquivalences] = topRight;
                countEquivalences++;
            }

            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == left){
                    insert = false;
                    break;
                }
            }
            if(left != 0 && insert){
                equivalences[labels[i]][countEquivalences] = left;
                countEquivalences++;
            }

            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == right){
                    insert = false;
                    break;
                }
            }
            if(right != 0 && insert){
                equivalences[labels[i]][countEquivalences] = right;
                countEquivalences++;
            }

            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == down){
                    insert = false;
                    break;
                }
            }
            if(down != 0 && insert){
                equivalences[labels[i]][countEquivalences] = down;
                countEquivalences++;
            }


            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == downLeft){
                    insert = false;
                    break;
                }
            }
            if(downLeft != 0 && insert){
                equivalences[labels[i]][countEquivalences] = downLeft;
                countEquivalences++;
            }

            insert = true;            
            for(int j=0;j<label;j++){
                if(equivalences[labels[i]][j] == downRight){
                    insert = false;
                    break;
                }
            }
            if(downRight != 0 && insert){
                equivalences[labels[i]][countEquivalences] = downRight;
                countEquivalences++;
            }
        }
    }    




    for(int i=0;i<size;i++){
        if(labels[i] != 0){
            for(int j=0;j<label;j++){
                if(labels[i] > equivalences[labels[i]][j] && equivalences[labels[i]][j] != 0){
                    labels[i] = equivalences[labels[i]][j];
                }
            }
        }

    }
    int upLabel = 0;
    for(int i=0; i<size;i++){
        if(labels[i] > upLabel){
            upLabel = labels[i];
        }
    }
    //next step is identify the limits of label for each label
    letter letters[upLabel];
    for(int i=0;i<=upLabel;i++){
        letters[i].top = letters[i].bottom = letters[i].left = letters[i].right = -1;
    }

    for(int i=0; i<size;i++){
        if(labels[i] != 0){
            int y = (int) (i+1)/info.width;
            int x = i%info.width;            
            letters[labels[i]].top    = ((y < letters[labels[i]].top || letters[labels[i]].top == -1) ? y : letters[labels[i]].top);
            letters[labels[i]].bottom = ((y > letters[labels[i]].bottom || letters[labels[i]].bottom == -1) ? y : letters[labels[i]].bottom);
            letters[labels[i]].left   = ((x < letters[labels[i]].left || letters[labels[i]].left == -1) ? x : letters[labels[i]].left);
            letters[labels[i]].right  = ((x > letters[labels[i]].right || letters[labels[i]].right == -1) ? x : letters[labels[i]].right);

        }
    }


    //identify the characters

    //implementation of feature extraction
    /*
    1 - Crossing in each letter
    In my crossing, I'm seeing the backgrounds amount (set of labels with rotule 0 for each set of labels with not zero rotule)
    */
    for(int i = 1; i<=upLabel;i++){
        //for each letter
        //check if really have a letter on here
        if(letters[i].top != 0 && letters[i].bottom != 0){
            //calculate the size of letter inside the vector
            std::string arquivo = "/sdcard/letter"+std::to_string(i)+".txt";

            FILE * letterFile = fopen(arquivo.c_str(),"w+");
            for(int y=letters[i].top;y<letters[i].bottom;y++){
                int changes = 0;
                for(int x=letters[i].left;x<letters[i].right;x++){
                    if(labels[info.width*y+x] != labels[info.width*y+x+1]){
                        changes++;
                    }
                    fprintf(letterFile,"%d",labels[info.width*y+x]);
                }

                fprintf(letterFile," - %d \n",changes);
            }

            fclose(letterFile);


        }
    }

    


    //just for log
    FILE * labelFile = fopen("/sdcard/labels.txt","w+");
    for(int i=1;i<=upLabel;i++){        
        fprintf(labelFile, "Label: %d - TOP: %d | BOTTOM: %d | LEFT: %d | RIGHT: %d \n",i,letters[i].top,letters[i].bottom,letters[i].left,letters[i].right);
    }
    fclose(labelFile);

    FILE * arquivo = fopen("/sdcard/out.txt","w+");
    for(int i=0;i<(int) info.height;i++){
        for(int j=0;j<(int)info.width;j++){
            fprintf(arquivo, "%d ", labels[(info.width*i+j)]);    
        }
        fprintf(arquivo, "\n");    
    }
    fclose(arquivo);
}




void nativeImageToBlackAndWhite(JNIEnv *env,jlong dataPointerLong,jobject bitmap){
    auto dataPointer = (NativeData *) dataPointerLong;
    AndroidBitmapInfo info;    
    int ret;
    if((ret = AndroidBitmap_getInfo(env,bitmap,&info)) < 0){
        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Image error: %d",ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888){
        __android_log_print(ANDROID_LOG_VERBOSE, "LogCpp", "Image Format Error");
        return;
    }

    long imageSize = info.height*info.width;
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