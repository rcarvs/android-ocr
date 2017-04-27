/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef parallelus_NETWORK_HPP
#define parallelus_NETWORK_HPP

#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <thread>
#include <jni.h>
#include "util/error.h"
#include <parallelus/NetworkManager.hpp>
#include "NetWorker.hpp"

namespace parallelus {
class NetWorker;

class Network {
	
    std::mutex _mutexCC, _mutexCW;
    std::condition_variable _cvCC, _cvCW;
    std::shared_ptr<NetworkManager> _netManager;
    std::vector<std::shared_ptr<NetWorker>> _networkers;
    std::shared_ptr<Scheduler> _netScheduler;
    std::string _kernelsource;	//Sould Be a list of strings
    bool _killCC, _killCW;
    bool _runningCC, _runningCW;
    std::shared_ptr<Device> _device;
    
    public:
  
		Network(std::shared_ptr<Scheduler> netScheduler, std::shared_ptr<Device> device): _killCC(false), 
				_runningCC(false), _device(device){ 
			_netScheduler = netScheduler;
			_killCW = false;
			_runningCW = false;
			_netManager = std::make_shared<NetworkManager>();
			_kernelsource = "";
			
		}
		
		~Network() {
			std::unique_lock<std::mutex> lockCC(_mutexCC);
			std::unique_lock<std::mutex> lockCW(_mutexCW);
			_cvCC.notify_one();
			_cvCW.notify_one();
			_killCC = true;
			_killCW = true;
		}
		
		void networkManagerRun(JavaVM *jvm) {
			networkControlConections(jvm);
			networkControlNetWorkers(jvm);
		}
    
		/*
		 * Thread responsible to controu incomming socket conections
		 */
		void networkControlConections(JavaVM *jvm) {
			if(_runningCC)
				return;
			_runningCC = true;

			std::thread controlConectionsThread([=] () mutable {
				JNIEnv *env = nullptr;

				if(jvm) {
					if(jvm->AttachCurrentThread(&env, nullptr))
						throw std::runtime_error("[NETWORK]failed to attach thread to JVM.");
					//_device->setJNIEnv(env);
				}

				std::unique_lock<std::mutex> lockCC(_mutexCC);
								 
				 ERROR_PRINTER_FUNC("[NETWORK]Control Conections Thread Created.");
				 
				 _netManager->initNetwork();
				 
				while(true){
					int newsockfd = _netManager->acceptConections();
					_networkers.push_back(std::make_shared<NetWorker>(newsockfd,_kernelsource,_device));
					_networkers.back()->run(_netScheduler, jvm);
					sendKernelSource(_networkers.back()->getSocketNumber());
				}
				 
				/*
				 * Network 
				 */ 
				if(jvm)
					jvm->DetachCurrentThread();
			});
			controlConectionsThread.detach();
		}
		
		void networkControlNetWorkers(JavaVM *jvm) {
			if(_runningCW)
				return;
			_runningCW = true;

			std::thread controlNetWorkersThread([=] () mutable {
				JNIEnv *env = nullptr;

				if(jvm) {
					if(jvm->AttachCurrentThread(&env, nullptr))
						throw std::runtime_error("[NETWORK]failed to attach thread to JVM.");
					//_device->setJNIEnv(env);
				}

				std::unique_lock<std::mutex> lockCW(_mutexCW);
							 
				ERROR_PRINTER_FUNC("[NETWORK]Control NetWorkers Created.");
				
				if(jvm)
					jvm->DetachCurrentThread();
			});
			controlNetWorkersThread.detach();
		}
		
		void setKernelString(const char *source){
		
			_kernelsource += source; //Review this
		
		}
		
		void sendKernelToAll(){
			for(auto &networker : _networkers)
				sendKernelSource(networker->getSocketNumber());
		}
		
		void sendKernelSource(int socketfd){
			auto net = new NetworkManager(socketfd);
			net->sendString("KERNEL____");			//TAG
			net->sendInt(_kernelsource.length());		//kernel name size
			net->sendString(_kernelsource.c_str());	//kernel name string
		}
		
		void wakeUpNetWorkers(){
			for(auto &worker : _networkers)
				worker->wakeUp();
		}
		
		void finishNetWorkers(){
			for(auto &worker : _networkers)
				worker->finish();
		}
		
	};

}

#endif // !parallelus_NETWORK_HPP
