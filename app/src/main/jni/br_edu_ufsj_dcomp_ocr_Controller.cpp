#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <parallelme/ParallelME.hpp>
#include <parallelocr/ParallelOCR.hpp>
#include "br_edu_ufsj_dcomp_ocr_Controller.h"

using namespace parallelme;
using namespace parallelocr;


const static char gKernels[] =     
"__kernel void crossing(__global uint *letter,__global uint *width,__global uint *ccount){	\n"
"	uint id = get_global_id(0);														        \n"
"	uint changes = 0;																        \n"
"	for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){						        \n"
"		if(letter[i] != letter[i+1]){												        \n"
"			changes++;																        \n"
"		}																			        \n"
"	}																				        \n"
"	ccount[id] = changes;														            \n"
"}																					        \n"
"																					        \n"
"__kernel void blackandwhite(__global uchar4 *image) {      						        \n"
"    int gid = get_global_id(0);                            						        \n"
"    uchar4 pixel = image[gid];                             						        \n"
"    if((pixel.x+pixel.y+pixel.z) > 645){                   						        \n"
"       pixel.x = pixel.y = pixel.z = 255;                      					        \n"
"    }else{                                                     					        \n"
"       pixel.x = pixel.y = pixel.z = 0;                        					        \n"
"    }                                                          					        \n"
"    image[gid] = pixel;                                        					        \n"
"}                                                          						        \n"
"																					        \n";





struct NativeData{
    std::shared_ptr<Runtime> runtime;
    std::shared_ptr<Program> program;
};



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



JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeCreateImageLabels(JNIEnv *env,jobject self,jlong dataPointerLong,jobject bitmap,jobject textViewOutOut){
    jclass clazz = (*env).FindClass("android/widget/TextView");
    jmethodID setText = (*env).GetMethodID(clazz, "setText", "(Ljava/lang/CharSequence;)V");
    //Initialize image class with pointer to image
    Image image(env,&bitmap);
    //Set the runtime e program objects in Image class
    auto dataPointer = (NativeData *) dataPointerLong;
    image.setRuntime(dataPointer->runtime);
    image.setProgram(dataPointer->program);

    //first pass will put image in black and white
    image.bitmapTransformBlackAndWhite();
    //second pass is extract the rgb values from each pixel
    image.extractPixelsRGB();
    //now the function to label release all work for rotule and release a feature extraction
    //why the feature extraction are in same function of labeling???
    //because the feature extraction is in matrix search time :D :D :D
    image.toLabel();





    jstring jstr = (*env).NewStringUTF( "This comes from jni.");
    (*env).CallVoidMethod( textViewOutOut, setText, jstr);
}