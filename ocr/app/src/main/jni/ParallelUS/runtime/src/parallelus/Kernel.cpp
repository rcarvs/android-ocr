/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <parallelus/Kernel.hpp>
#include <parallelus/Buffer.hpp>
#include <parallelus/Device.hpp>
#include <parallelus/Program.hpp>
#include <parallelus/NetworkManager.hpp>
#include <string>
#include "dynloader/dynLoader.h"
#include "util/error.h"
using namespace parallelus;

Kernel::Kernel(const std::string &name, std::shared_ptr<Device> device,
        Program &program) : _device(device) {
    int err;
    
    _name = name;

    _clKernel = clCreateKernel(program.clProgram(device->id()), name.c_str(), &err);
    if(err < 0)
        throw KernelConstructionError(std::to_string(err));
}

Kernel::Kernel(const std::string &name, JNIEnv *env) {
	_name = name;
	_env = env;
    _clKernel = nullptr;
}

Kernel::~Kernel() {
    if(_clKernel) {
        clReleaseKernel(_clKernel);
        _clKernel = nullptr;
    }
}

void Kernel::run() {
    size_t offset[] = { 0, 0, 0 };
    size_t workSize[] = { _xDim, _yDim, _zDim };
    int err = clEnqueueNDRangeKernel(_device->clQueue(), _clKernel, 3, offset,
            workSize, nullptr, 0, nullptr, nullptr);
    if(err < 0)
        throw KernelExecutionError(std::to_string(err));
}

void Kernel::sendRunCommand(){
	
	
}

Kernel *Kernel::setArg(unsigned id, std::shared_ptr<Buffer> buffer, int type) {
    		
    buffer->setId(id);
    buffer->setKernelName(_name);

    if(type != 0){ 
        
        /* If is an extern execution: send all over socket */
        buffer->setSocketNumber(type);
        auto net = new NetworkManager(type);
        buffer->setExtern();
        buffer->createPointer(_env);
		
        net->sendString("ARG_______");                                              //TAG: ARG_______
        net->sendInt(_name.length());                                               //Kernel name size
        net->sendString(_name.c_str());                                             //Kernel name string              
        net->sendInt(1); //1 = Buffer arg // 0 = Primitive Arg 
        net->sendUnsignedInt(buffer->getBufferSize()); 
        net->sendInt(buffer->getBufferId());                              //Argument Size
        if(buffer->getBufferPointer() == nullptr){
            net->sendInt(1); // [true=1] = Empty buffer (output) 
        }
        else{
            net->sendInt(0); // [true=0] = Filled Buffer (inputs) 
            net->sendVoidBuffers(buffer->getBufferPointer(), buffer->getBufferSize());  //Argument Data 
        }
    }
    else{ 
        /* If is an inter execution: use opencl */
        int err = type;
        auto mem = buffer->clMem(_device);

        err = clSetKernelArg(_clKernel, id, sizeof(mem), &mem);
        if(err < 0)
            throw KernelArgError(std::string("Buffer error: ") + std::to_string(err));
    }
    return this;
}

Kernel *Kernel::setPrimitiveArg(unsigned id, size_t size, void *host, int type) {
    
    if(type != 0){
            auto net = new NetworkManager(type);
            net->sendString("ARG_______");                                              //TAG: ARG_______
            net->sendInt(_name.length());                                               //Kernel name size
            net->sendString(_name.c_str());
            net->sendInt(0); //1 = Buffer arg // 0 = Primitive Arg 
            net->sendUnsignedInt(size); 
            net->sendInt((int)id);
            net->sendInt(0); // [true=0] = Filled Buffer (inputs) 
            net->sendVoidBuffers(host, size);  //Argument Data 
    }
    else{
        int err = type;

        err = clSetKernelArg(_clKernel, id, size, host);
        if(err < 0)
        throw KernelArgError(std::string("Primitive error: ") + std::to_string(err));        
    }
    return this;
}

