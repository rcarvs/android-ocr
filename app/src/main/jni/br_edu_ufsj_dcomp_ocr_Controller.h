//
// Created by labpi on 11/01/17.
//

#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_BR_EDU_UFSJ_DCOMP_OCR_CONTROLLER_H
#define OPTICALCHARACTERRECOGNITIONPARALLEL_BR_EDU_UFSJ_DCOMP_OCR_CONTROLLER_H
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jlong JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeInit(JNIEnv *env, jobject self);
JNIEXPORT void JNICALL Java_br_edu_ufsj_dcomp_ocr_Controller_nativeCreateImageLabels(JNIEnv *env,jobject self,jlong dataPointerEnv,jobject bitmap);

#ifdef __cplusplus
}
#endif
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_BR_EDU_UFSJ_DCOMP_OCR_CONTROLLER_H
