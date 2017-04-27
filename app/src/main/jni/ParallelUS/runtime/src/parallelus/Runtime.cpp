/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <parallelus/Runtime.hpp>
#include <parallelus/Task.hpp>
#include "Worker.hpp"
#include "Network.hpp"
#include "dynloader/dynLoader.h"
using namespace parallelus;

Runtime::~Runtime() = default;

void Runtime::loadDevices() {
    if(!dynLoadOpenCL())
        throw RuntimeConstructionError("No OpenCL devices found.");

    int err;

    // Get the number of platforms.
    unsigned numPlatforms;
    err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    if(err < 0)
        throw RuntimeConstructionError(std::to_string(err));

    // Get the platforms.
    auto platforms =
        std::unique_ptr<cl_platform_id []>{new cl_platform_id[numPlatforms]};
    err = clGetPlatformIDs(numPlatforms, platforms.get(), nullptr);
    if(err < 0)
        throw RuntimeConstructionError(std::to_string(err));

    // Initialize the devices for each platform.
    for(unsigned i = 0; i < numPlatforms; ++i) {
        unsigned numDevices;
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, nullptr,
                &numDevices);
        if(err < 0)
            throw RuntimeConstructionError(std::to_string(err));

        std::unique_ptr<cl_device_id []> devices{new cl_device_id[numDevices]};
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, numDevices,
                devices.get(), nullptr);
        if(err < 0)
            throw RuntimeConstructionError(std::to_string(err));

        for(unsigned j = 0; j < numDevices; ++j)
            _devices.push_back(std::make_shared<Device>(devices[j]));
    }

}

void Runtime::startWorkers(JavaVM *jvm) {
    for(auto &device : _devices)
        _workers.push_back(std::make_shared<Worker>(device));
    for(auto &worker : _workers)
        worker->run(_scheduler, jvm);
}

void Runtime::startNetwork(std::shared_ptr<Scheduler> _netScheduler, JavaVM *jvm) {
    std::shared_ptr<Device> networkeDevice = std::make_shared<Device>();
	_network = std::make_shared<Network>(_netScheduler,networkeDevice);
	_network->networkManagerRun(jvm);
}

void Runtime::submitTask(std::unique_ptr<Task> task, int execType) {
	
	/*
	 * Decide here if task goes to inter execution or extern
	 */ 
	 if(!_useNetwork){ //Net not activated, so, just intern execution
		_scheduler->push(std::move(task));
		for(auto &worker : _workers)
			worker->wakeUp();
	 }
	 else if(_useNetwork){
        if(execType == 2){ //Just Inter execution
            _scheduler->push(std::move(task));
            for(auto &worker : _workers)
                worker->wakeUp();
        }

        if(execType == 3){ //Just extern execution
            _netScheduler->push(std::move(task));
            _network->wakeUpNetWorkers();
        }

        if(execType == 1){ //"Scheduled"
            if(_scheduler->workersBusy(20) && !_netScheduler->workersBusy(1)){ //If inter has more than 2 tasks, send to extern
                _netScheduler->push(std::move(task));
                _network->wakeUpNetWorkers();
            }
            else{ //If inter is ok, execute
                _scheduler->push(std::move(task));
                for(auto &worker : _workers)
                    worker->wakeUp();
            }
        }
	 }
}

void Runtime::finish() {
    _scheduler->waitUntilIdle();
    for(auto &worker : _workers)
        worker->finish();

    if(_useNetwork){
    	_network->finishNetWorkers();
    	_netScheduler->waitUntilIdle();
    }
}
