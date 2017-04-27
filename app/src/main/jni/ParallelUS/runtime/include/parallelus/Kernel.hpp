/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef parallelus_KERNEL_HPP
#define parallelus_KERNEL_HPP

#include <cstdlib>
#include <string>
#include <stdexcept>

#include "../../src/parallelus/util/error.h"

#include "NetworkManager.hpp"

struct _cl_kernel;

namespace parallelus {
class Buffer;
class Device;
class Program;
class Task;
class NetworkManager;
/**
 * Exception thrown if the kernel failed execute.
 * The error message can be accessed through the what() function.
 */
class KernelExecutionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Exception thrown if the kernel failed to be created.
 * The error message can be accessed through the what() function.
 */
class KernelConstructionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Exception thrown if the kernel failed to configure the argument.
 * The error message can be accessed through the what() function.
 */
class KernelArgError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * This class represents a Task's kernel.
 *
 * @author Renato Utsch
 */
class Kernel {
    friend class Task;

    /**
     * Executes the kernel with the given work size.
     */
    void run();
    
    void sendRunCommand();

    /**
     * Constructs the Kernel object. Only the Task class can do it.
     * To create a kernel to be used with a task, call the Task::addKernel()
     * function.
     * @see Task::addKernel
     */
    Kernel(const std::string &name, std::shared_ptr<Device> device, Program &program);
    
    Kernel(const std::string &name, JNIEnv *env);

public:
    Kernel(const Kernel &) = delete;
    Kernel &operator=(const Kernel &) = delete;

    ~Kernel();

    /**
     * Sets the range of execution.
     * @param xDim The number of items in the x dimension.
     * @param yDim The number of items in the y dimension.
     * @param zDim The number of items in the z dimension.
     */
    inline Kernel *setWorkSize(int xDim, int yDim, int zDim, int type) {
        _xDim = xDim;
        _yDim = yDim;
        _zDim = zDim;
        
         if(type != 0){
            auto net = new NetworkManager(type);	
			net->sendString("WORKSIZE__");	//TAG
			net->sendInt(_name.length());	//Kernel name size
			net->sendString(_name.c_str());	//Kernel name string
			net->sendUnsignedInt(_xDim); 	//uint value: Xdim
			net->sendUnsignedInt(_yDim); 	//uint value: Ydim
			net->sendUnsignedInt(_zDim); 	//uint value: Zdim
		}
		return this;
	}

    /**
     * Sets a buffer as the argument with the given id.
     * Must only be called inside a ConfigFunction.
     */
    Kernel *setArg(unsigned id, std::shared_ptr<Buffer> buffer, int type);

    /**
     * Sets a primitive type as the argument with the given id.
     * Must only be called inside a ConfigFunction.
     * @see Task::ConfigFunction
     */
    template<typename T>
    Kernel *setArg(unsigned id, T primitive, int type) {
        return setPrimitiveArg(id, sizeof(primitive), &primitive, type);
    }

    /// Returns the the kernel's x dimension work items.
    inline size_t xDim() {
        return _xDim;
    }

    /// Returns the the kernel's y dimension work items.
    inline size_t yDim() {
        return _yDim;
    }

    /// Returns the the kernel's z dimension work items.
    inline size_t zDim() {
        return _zDim;
    }

private:
    /**
     * Sets a PRIMITIVE type as the argument to the kernel. Trying to use this
     * function with other types will cause a segfault.
     */
    Kernel *setPrimitiveArg(unsigned id, size_t size, void *host, int type);

    std::shared_ptr<Device> _device;
    _cl_kernel *_clKernel;
    size_t _xDim, _yDim, _zDim;
    std::string _name;
    JNIEnv *_env;
};

}

#endif // !parallelus_KERNEL_HPP
