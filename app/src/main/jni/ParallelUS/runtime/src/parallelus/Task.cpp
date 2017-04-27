/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <parallelus/Task.hpp>
#include <parallelus/Device.hpp>
#include <parallelus/Kernel.hpp>
#include <parallelus/Program.hpp>
#include <parallelus/NetworkManager.hpp>
#include "util/error.h"
using namespace parallelus;

Task::Task(std::shared_ptr<Program> program, Score score) : _score(score),
        _configFunction(nullptr), _program(program) {

}

Task::~Task() = default;

Task *Task::addKernel(const std::string &name) {
    _kernelNames.push_back(name);
    return this;
}

void Task::createKernels(std::shared_ptr<Device> &device) {
    for(auto &name : _kernelNames) {
        // I don't use std::make_shared here because Kernel's constructor is private.
        auto kernel = std::shared_ptr<Kernel>(new Kernel(name, device, *_program));

        _kernels.push_back(kernel);
        _kernelHash.insert(std::pair<std::string, Kernel *>(name, kernel.get()));
    }
}

void Task::createExternKernels(JNIEnv *env) {
    for(auto &name : _kernelNames) {
        // I don't use std::make_shared here because Kernel's constructor is private.
        auto kernel = std::shared_ptr<Kernel>(new Kernel(name,env));

        _kernels.push_back(kernel);
        _kernelHash.insert(std::pair<std::string, Kernel *>(name, kernel.get()));
    }
}

void Task::sendKernelsName(int socketfd){
	auto net = new NetworkManager(socketfd);
	
	for(auto &kernelName: _kernelNames){
		net->sendString("KERNELNAME");		//TAG
		net->sendInt(kernelName.length());	//Kernel name size
		net->sendString(kernelName.c_str());	//Kernel name string
	}
}

void Task::run() {
    for(auto &kernel : _kernels)
        kernel->run();
}

void Task::runExtern(int sockfd) {
	
	ERROR_PRINTER_FUNC("[TASK]Execute command sent.");
	auto net = new NetworkManager(sockfd);
	net->sendString("EXECUTE___");	//TAG
	
	char *msg = (char*)net->receiveVoidBuffer(10*sizeof(char));
	if(strcmp(msg,"DONE______") == 0){
		ERROR_PRINTER_FUNC("[TASK]NetWorker finished execution.");
		/* Whem networker indicates that execution finished, just exit this function */
	}
	free(msg);
	//Wait Response
}

