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
"__kernel void identification(\n"
"	__global uint *trainData,\n"
"	__global uint *dataSize,\n"
"	__global uint *changes,\n"
"	__global uint *changesSize,\n"
"	__global uint *rotule,\n"
"	__global uint *result){\n"
"\n"
"	//first pass is normalize the trainData passing the normalized version to rotule\n"
"	uint crossingRotuleSize = 0;\n"
"	for(uint i=0;i<(changesSize[0]-1);i++){\n"
"		if(changes[i] != changes[(i+1)]){\n"
"			rotule[crossingRotuleSize] = changes[i];\n"
"			crossingRotuleSize++;\n"
"		}	\n"
"	}\n"
"	//second pass is compare the rotule with the trainData to see if will match with any letter\n"
"	uint contRotules = 0;\n"
"	uint letterofTime = 0;\n"
"	for(uint i=0;i<dataSize[0];i++){		\n"
"		letterofTime = trainData[i];\n"
"		i++;\n"
"		contRotules = trainData[i];\n"
"		if(contRotules == crossingRotuleSize){\n"
"			//if that to occur i need to compare each position of vector\n"
"			uint equals = 1;\n"
"			for(uint j=1;j<=contRotules;j++){\n"
"				if(trainData[i+j] != rotule[(j-1)]){\n"
"					equals = 0;\n"
"					break;\n"
"				}\n"
"			}\n"
"			if(equals == 1){\n"
"				result[0] = letterofTime;\n"
"				break;\n"
"			}\n"
"			\n"
"		}\n"
"		i += contRotules;\n"
"		contRotules = 0;		\n"
"	}	\n"
"}\n"
"__kernel void crossing(__global uint *letter,__global uint *width,__global uint *ccount){																\n"
"	uint id = get_global_id(0);														        															\n"
"	uint changes = 0;																        															\n"
"	for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){						        															\n"
"		if(letter[i] != letter[i+1]){												        															\n"
"			changes++;																        															\n"
"		}																			        															\n"
"	}																				        															\n"
"	ccount[id] = changes;														            															\n"
"}																					        															\n"
"																					        															\n"
"__kernel void blackandwhite(__global uchar4 *image,__global uint *r,__global uint *g,__global uint *b,__global uint *label,__global uint *checked) {   \n"
"    int gid = get_global_id(0);                            						        															\n"
"    uchar4 pixel = image[gid];                             						        															\n"
"    label[gid] = checked[gid] = 0;																														\n"
"    if((pixel.x+pixel.y+pixel.z) > 645){                   						        															\n"
"		r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 255;                      																\n"
"    }else{                                                     					        															\n"
"    	r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 0;                        																\n"
"    }                                                          					        															\n"
"    image[gid] = pixel;                                        					        															\n"
"} 																																						\n";


struct NativeData{
    std::shared_ptr<Coach> coach;
    std::shared_ptr<Runtime> runtime;
    std::shared_ptr<Program> program;
};


JNIEXPORT jlong JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeInit(JNIEnv *env, jobject self, jobject javaAssetManager) {
    JavaVM *jvm;
    env->GetJavaVM(&jvm);
    if(!jvm) return (jlong) nullptr;

    auto dataPointer = new NativeData();
    dataPointer->coach = std::make_shared<Coach>(env,javaAssetManager);
    dataPointer->runtime = std::make_shared<Runtime>(jvm,std::make_shared<SchedulerFCFS>());
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

    //for print phrase
    std::string phrase = "";
    for(unsigned int i = 0;i<image.getLetterCount();i++){
        phrase = phrase+image.getLetter(i)._letter;
    }


    jstring result = (*env).NewStringUTF(phrase.c_str());
    (*env).CallVoidMethod( textViewOutOut, setText, result);
}