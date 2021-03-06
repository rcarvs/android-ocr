/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <parallelus/SchedulerFCFS.hpp>
#include <parallelus/Device.hpp>
#include <parallelus/Program.hpp>
#include <parallelus/Task.hpp>


#include <unistd.h>
#include <android/log.h>
using namespace parallelus;

void SchedulerFCFS::push(std::unique_ptr<Task> task){
    std::unique_lock<std::mutex> lock(_mutex);
    _taskList.push_back(std::move(task));
}

std::unique_ptr<Task> SchedulerFCFS::pop(Device &device){
    std::unique_lock<std::mutex> lock(_mutex);

    if(!_taskList.empty()
            && _taskList.front()->program().hasDeviceID(device.id())) {
        std::unique_ptr<Task> retTask = std::move(_taskList.front());
        _taskList.pop_front();
        return retTask;
    }
    else {
        _cv.notify_all();
        return nullptr;
    }
}

std::unique_ptr<Task> SchedulerFCFS::netpop(){
	
    std::unique_lock<std::mutex> lock(_mutex);
    __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui999");
    if(!_taskList.empty()) {
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9991");
        std::unique_ptr<Task> retTask = std::move(_taskList.front());
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9992");
        _taskList.pop_front();
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9993");
        return retTask;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9994");
        _cv.notify_all();
        __android_log_print(ANDROID_LOG_INFO, "Teste", "Entrou aqui9995");
        return nullptr;
    }
}

void SchedulerFCFS::waitUntilIdle() {
    std::unique_lock<std::mutex> lock(_mutex);
    for(;;) {
        if(_taskList.empty())
            break;

        _cv.wait(lock);
    }
}

bool SchedulerFCFS::workersBusy(int busyRate){
    if((int)_taskList.size() >= busyRate) return true;
    return false;
}
