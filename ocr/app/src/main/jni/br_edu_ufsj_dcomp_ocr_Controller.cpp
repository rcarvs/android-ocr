#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <parallelus/ParallelUS.hpp>
#include <parallelocr/ParallelOCR.hpp>
#include "br_edu_ufsj_dcomp_ocr_Controller.h"
#include <time.h>
#include <mutex>

using namespace parallelus;
using namespace parallelocr;


const static char gKernels[] = "__kernel void test1(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test2(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test3(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test4(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test5(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test6(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test7(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test8(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test9(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test10(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test11(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test12(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test13(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test14(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void test15(__global uint *x,__global uint *y) { int gid = get_global_id(0); x[gid] = gid;y[gid] = gid;} __kernel void crossing(__global uint *letter,__global uint *width,__global uint *ccount){ uint id = get_global_id(0); uint changes = 0; for (uint teste = 0;teste < 1;teste++){ changes = 0; for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){ if(letter[i] != letter[i+1]){ changes++; } } } ccount[id] = changes; } __kernel void blackandwhite(__global uchar4 *image,__global uint *r,__global uint *g,__global uint *b,__global uint *label,__global uint *checked) { int gid = get_global_id(0); uchar4 pixel = image[gid]; label[gid] = checked[gid] = 0; if((pixel.x+pixel.y+pixel.z) > 645){ r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 255; }else{ r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 0; } image[gid] = pixel; }";
struct NativeData{
    std::shared_ptr<Coach> coach;
    std::shared_ptr<Runtime> runtime;
    std::shared_ptr<Program> program;
    long double start_time;
    clock_t begin;
    clock_t end;
};


JNIEXPORT jlong JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeInit(JNIEnv *env, jobject self, jobject javaAssetManager) {

    clock_t begin = clock();
    JavaVM *jvm;
    env->GetJavaVM(&jvm);
    if(!jvm) return (jlong) nullptr;

    auto dataPointer = new NativeData();

    dataPointer->begin = clock();
    dataPointer->coach = std::make_shared<Coach>(env,javaAssetManager);
    dataPointer->runtime = std::make_shared<Runtime>(jvm,true);
    dataPointer->program = std::make_shared<Program>(dataPointer->runtime, gKernels);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    dataPointer->start_time = elapsed_secs;


    __android_log_print(ANDROID_LOG_INFO, "Evaluation Time", "Program Initialization: %f",elapsed_secs);

    return (jlong) dataPointer;
}

JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeCreateImageLabels(JNIEnv *env,jobject self,jlong dataPointerLong,jobject bitmap,jobject textViewOutOut){
    auto dataPointer = (NativeData *) dataPointerLong;
    dataPointer->begin = clock();

    JavaVM *jvm;
    env->GetJavaVM(&jvm);

    jclass clazz = (*env).FindClass("android/widget/TextView");
    jmethodID setText = (*env).GetMethodID(clazz, "setText", "(Ljava/lang/CharSequence;)V");
    //Initialize image class with pointer to image

    //Set the runtime e program objects in Image class
    Image image(env,&bitmap,jvm);
    image.setCoach(dataPointer->coach);
    image.setRuntime(dataPointer->runtime);
    image.setProgram(dataPointer->program);

    //first pass will put image in black and white
    image.bitmapTransformBlackAndWhite();
    //second pass is extract the rgb values from each pixel
    //image.extractPixelsRGB();
    //now the function to label release all work for rotule and release a feature extraction
    //why the feature extraction are in same function of labeling???
    //because the feature extraction is in matrix search time :D :D :D
    image.toLabel();
    dataPointer->end = clock();

    double elapsed_secs = double(dataPointer->end - dataPointer->begin) / CLOCKS_PER_SEC;
    __android_log_print(ANDROID_LOG_INFO, "Evaluation Time", "Program Total Time: %Lf",dataPointer->start_time+elapsed_secs);

    //for print phrase
    std::string phrase = "";
    /*for(unsigned int i = 0;i<image.getLetterCount();i++){
        phrase = phrase+image.getLetter(i)._letter;
    }*/


    jstring result = (*env).NewStringUTF(phrase.c_str());
    (*env).CallVoidMethod( textViewOutOut, setText, result);
}