/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef parallelus_NETWORKER_HPP
#define parallelus_NETWORKER_HPP

#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <thread>
#include <jni.h>
#include <parallelus/Kernel.hpp>
#include <parallelus/Task.hpp>

#include "util/error.h"

namespace parallelus {

/**
 * This class manages a threads that executes tasks supplied by the scheduler and send its 
 *
 * @author Guilherme Andrade
 */
class NetWorker {
    int _newsockfd;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::string _kernelsource;
    bool _kill;
    bool _running;
    JNIEnv *_env;

    std::shared_ptr<Device> _device;

    void executeTask(std::unique_ptr<Task> task) {
        ERROR_PRINTER_FUNC("[NETWORKER]Task execution initialized.");
        task->createExternKernels(_env);
        task->sendKernelsName(_newsockfd);
        task->callConfigFunction(_device,_newsockfd);
        task->runExtern(_newsockfd);	
        task->callFinishFunction(_device,_newsockfd);
        ERROR_PRINTER_FUNC("[NETWORKER]Task execution finished.");
    }

public:
    /**
     * Constructs the NetWorker from the given device.
     */
    NetWorker(int newsockfd, std::string kernelsource, std::shared_ptr<Device> device) : _newsockfd(newsockfd), 
				_kernelsource(kernelsource), _kill(false), _running(false), _device(device) {

    }

    ~NetWorker() {
        /// Kills the NetWorker, but only if it was without anything to do.
        /// This will block until the NetWorker is killed.
        std::unique_lock<std::mutex> lock(_mutex);
        _kill = true;
        _cv.notify_one();
    }

    /**
     * Starts the NetWorker. If the NetWorker has already started this function does
     * nothing.
     * The NetWorker asks for work to the scheduler until the sheduler returns a
     * nullptr pointer to a task. When this happens, it enters into sleep until
     * it is waken up by wakeUp() or killed by kill() by the runtime.
     */
    template<class Scheduler>
    void run(std::shared_ptr<Scheduler> scheduler, JavaVM *jvm) {
        if(_running)
            return;
        _running = true;

        std::thread t([=] () mutable {
            _env = nullptr;

            if(jvm) {
                if(jvm->AttachCurrentThread(&_env, nullptr))
                    throw std::runtime_error("failed to attach thread to JVM.");
                _device->setJNIEnv(_env);    
            }

            ERROR_PRINTER_FUNC("[NETWORKER]NetWorker Running: %d", _newsockfd);
            std::unique_lock<std::mutex> lock(_mutex);
           
            for(;;) {	
                ERROR_PRINTER_FUNC("[NETWORKER]Taking task.");
                auto task = scheduler->netpop();
                ERROR_PRINTER_FUNC("[NETWORKER]Returned netpop");
                if(task) {
                    ERROR_PRINTER_FUNC("[NETWORKER]ExecuteTask");
                    executeTask(std::move(task));
                    ERROR_PRINTER_FUNC("[NETWORKER]Executed.");
                    continue;
                }
                __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9996");
                if(_kill)
                    return;
                __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9998");

                _cv.wait(lock);
                __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9999");
            }

            if(jvm)
                jvm->DetachCurrentThread();
        });
        t.detach();
    }
	
	int getSocketNumber(){
		return _newsockfd;
	}
		
    /// Waits for the NetWorker to finish.
    inline void finish() {
        std::unique_lock<std::mutex> lock(_mutex);
    }

    /**
     * Wakes up the NetWorker if it was sleeping because it didn't have anything
     * to do.
     */
    inline void wakeUp() {
        _cv.notify_one();
    }
    
};

}

#endif // !parallelus_NETWORKER_HPP
