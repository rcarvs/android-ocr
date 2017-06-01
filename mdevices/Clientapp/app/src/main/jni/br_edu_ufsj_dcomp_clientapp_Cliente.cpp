//
// Created by labpi on 01/06/17.
//
#include <parallelus/ParallelUS.hpp>
#include "br_edu_ufsj_dcomp_clientapp_Cliente.hpp"

using namespace parallelus;

struct NativeData {
	std::shared_ptr<Runtime> runtime;
	std::shared_ptr<Program> program;
	std::shared_ptr<NetworkManager> network;
};

JNIEXPORT jlong br_edu_ufsj_dcomp_clientapp_Cliente_nativeInit(JNIEnv *env, jobject self, jlong dataLong) {
    JavaVM *jvm;
    env->GetJavaVM(&jvm);
    if(!jvm) return (jlong) nullptr;
    auto dataPointer = new NativeData();
    dataPointer->runtime = std::make_shared<Runtime>(jvm,false);
    dataPointer->network = std::make_shared<NetworkManager>();
    return (jlong) dataPointer;
}
