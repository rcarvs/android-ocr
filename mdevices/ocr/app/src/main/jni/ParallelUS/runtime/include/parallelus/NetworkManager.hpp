/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#ifndef parallelus_NETWORKMANAGER_HPP
#define parallelus_NETWORKMANAGER_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <jni.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <android/log.h>
#include <stddef.h>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <tuple>
#include <list>
#include "Buffer.hpp"


namespace parallelus {
	
typedef std::tuple<unsigned int, unsigned int, unsigned int> Dimensions;
typedef std::map<std::string, std::pair<std::list<std::shared_ptr<Buffer>>,Dimensions>>  KernelArgument;
typedef std::map<std::string, std::pair<std::list<std::shared_ptr<Buffer>>,Dimensions>>::const_iterator kernelArgument_it;
typedef std::list<std::shared_ptr<Buffer>>::const_iterator bufferList_it;

class NetworkManager {
	
	int _sockfd;
	struct sockaddr_in _serv_addr;
	struct hostent *_server;
	 
public: 

	NetworkManager(int _sockfd);
	NetworkManager();
	~NetworkManager();

	void initNetwork();
	int acceptConections();
	int createSocket();
	int conect(int portno, const char *serveraddr);

	void sendString(const char* buffer);
	void sendBitmap(uint32_t *bitmapBuffer, int bitmapSize);
	void sendVoidBuffers(void *buffer, int bufferSize);
	void sendInt(int value);
	void sendUnsignedInt(unsigned int value);

	uint32_t* receiveBitmap(int bufferSize);
	int receiveInt();
	unsigned int receiveUnsignedInt();
	void *receiveVoidBuffer(int bufferSize);

	std::string receiveKernelString();
	std::string receiveTagMessage();
	void receiveArgBuffer(KernelArgument &argsBuffers);
	void receiveWorkSize(KernelArgument &argsBuffers);

	void sendOutputBuffer(std::shared_ptr<parallelus::Buffer> outputBuffer);
	
};

}

#endif // parallelus_NETWORKMANAGER_HPP
