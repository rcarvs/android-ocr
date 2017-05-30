/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef parallelus_RUNTIME_HPP
#define parallelus_RUNTIME_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <jni.h>
#include "Scheduler.hpp"
#include "SchedulerFCFS.hpp"
#include "../../src/parallelus/util/error.h"

namespace parallelus {
class Device;
class Loader;
class Worker;
class Network;

/**
 * Exception thrown if the runtime failed to find at least one device from the
 * available execution engines or failed to initialize those devices and
 * respective workers.
 * The error message can be accessed through the what() function.
 */
class RuntimeConstructionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * Exception thrown if the runtime failed to find at least one device from the
 * available execution engines.
 * The error message can be accessed through the what() function.
 */
class RuntimeExecutionError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

/**
 * The Runtime class is responsible for managing all the contexts used by
 * parallelus's runtime to execute a given kernel. It encapsulates the OpenCL
 * structures.
 * When creating the runtime, a pointer to the JavaVM can be passed to the
 * constructor, along with changing the scheduler policy
 * to one different to the default (FCFS). To make the runtime run in an
 * Android device, at least the JavaVM pointer must be given.
 *
 * @author Renato Utsch and Guilherme Andrade
 */
class Runtime {
    std::vector<std::shared_ptr<Device>> _devices;      /// Vector of devices.
    std::vector<std::shared_ptr<Worker>> _workers;      /// Runtime workers.
    std::shared_ptr<Network> _network;					/// Network thread.
    std::shared_ptr<Scheduler> _scheduler;              /// Runtime scheduler.
    std::shared_ptr<Scheduler> _netScheduler;              /// Runtime scheduler.
    bool _useNetwork;

    /// Initializes the devices.
    void loadDevices();

    /// Starts the workers.
    void startWorkers(JavaVM *jvm);
    
    /// Starts the network.
    void startNetwork(std::shared_ptr<Scheduler> _netScheduler, JavaVM *jvm);

public:
    /**
     * Constructs the runtime.
     * @param jvm A pointer to the JavaVM. If this is specified, the worker
     * threads of the runtime will be linked to the JavaVM.
     * @param sched the scheduler to be used by the runtime, defaulting to
     * First Come First Served.
     */
    Runtime(JavaVM *jvm = nullptr,
	    bool netUsage = false,
            std::shared_ptr<Scheduler> &&sched = std::make_shared<SchedulerFCFS>(),
            std::shared_ptr<Scheduler> &&netSched = std::make_shared<SchedulerFCFS>())
            : _scheduler(std::move(sched)), _netScheduler(std::move(netSched)), _useNetwork(netUsage) {
        loadDevices();
        startWorkers(jvm);
        if(_useNetwork) startNetwork(_netScheduler,jvm);
    }

    Runtime(const Runtime &) = delete;
    Runtime &operator=(const Runtime &) = delete;

    ~Runtime();

    /**
     * Submits a task for execution. The runtime claims ownership to this class,
     * which will be deleted by it after the execution.
     */
    void submitTask(std::unique_ptr<Task> task, int execType);

    /**
     * Waits for all tasks to finish before returning.
     */
    void finish();

    /**
     * Returns the available devices from all platforms.
     */
    inline std::vector<std::shared_ptr<Device>> &devices() {
         return _devices;
    }
    
    std::shared_ptr<Network> getNetwork(){	
		return _network;
	}

    bool networkUsage(){
	return _useNetwork;
    }
    
};

}

#endif // parallelus_RUNTIME_HPP
