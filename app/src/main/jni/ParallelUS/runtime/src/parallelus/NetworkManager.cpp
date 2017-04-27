/*                                                _    __ ____
 *   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 *  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 *  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 *  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 *
 */

#include <parallelus/NetworkManager.hpp>
#include "util/error.h"

using namespace parallelus;

NetworkManager::~NetworkManager() = default;
NetworkManager::NetworkManager(){}

NetworkManager::NetworkManager(int sockfd){
	_sockfd = sockfd;
}

void NetworkManager::initNetwork(){
	
    __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]Creating Socket\n");
    struct sockaddr_in serv_addr;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]Socket Created %d\n", _sockfd);
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    int portno = 5775;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    int yes=1;

    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    	perror("setsockopt");
    	exit(1);
    }
    
    int e = bind(_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(e < 0) __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]Error on BIngind: %d\n", e);
    __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]Socket Configuration ok!\n");	
}

int NetworkManager::acceptConections(){
	socklen_t clilen;
    struct sockaddr_in cli_addr;
    listen(_sockfd,5);
    clilen = sizeof(cli_addr);
           
    __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]Accepting NetWorkers Conexions\n");
    int newsockfd = accept(_sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
    return newsockfd;
}

int NetworkManager::createSocket(){
    __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[SOCKET]Creating Socket..\n");
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sockfd < 0){
    	__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[SOCKET]socket() failed: %s\n", strerror(errno));
    	return 0;
    }
    else{
    	__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[SOCKET]Socket Created %d..\n", _sockfd);
    	return 1;
    }
}

int NetworkManager::conect(int portno, const char *serveraddr){
	__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[CONECT]Conecting in %s:%d\n",serveraddr,portno);
	_server = gethostbyname(serveraddr);

	if (_server == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[CONECT]ERROR, no such host\n");
		return 0;
	}

	bzero((char *) &_serv_addr, sizeof(_serv_addr));
	_serv_addr.sin_family = AF_INET;
	bcopy((char *)_server->h_addr, (char *)&_serv_addr.sin_addr.s_addr, _server->h_length);
	_serv_addr.sin_port = htons(portno);

	if (connect(_sockfd,(struct sockaddr *)&_serv_addr,sizeof(_serv_addr)) < 0){
		__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[CONECT]ERROR connecting");
		return 0;
	}
	__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[CONECT]Conection done!\n");
	return 1;
}

void NetworkManager::sendString(const char* buffer){
	int n;
	int bufferSize = strlen(buffer);
	int send = 0;
	while (send < bufferSize){
		if(send == 0)
			n = write(_sockfd,buffer,bufferSize);    
		else
			n = write(_sockfd,static_cast<const char *>(buffer)+(send/sizeof(char)),bufferSize-send);    
		 if (n < 0)  __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]ERROR writting from socket");
		send += n;
	}
}

void NetworkManager::sendBitmap(uint32_t *bitmapBuffer, int bitmapSize){
	int n;	
	int send = 0;
	while (send < bitmapSize){
		if(send == 0)
			n = write(_sockfd,bitmapBuffer,bitmapSize);
		else
			n = write(_sockfd,bitmapBuffer+(send/sizeof(uint32_t)),bitmapSize-send);
		 if (n < 0)  __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]ERROR writting from socket");
		send += n;
	}
}

void NetworkManager::sendVoidBuffers(void *buffer, int bufferSize){
	int n;
	int send = 0;
	
	while (send < bufferSize){
		//__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "Enviando de noooooooovo.....");
		if(send == 0){
			n = write(_sockfd,buffer,bufferSize); 
			//__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "Enviei %d de %d PORRA...",n,bufferSize);
		}   
		else{
			n = write(_sockfd,static_cast<char*>(buffer)+send,bufferSize-send);
			//__android_log_print(ANDROID_LOG_ERROR, "DEBUG", "Enviei %d de %d PORRA...",n,bufferSize);
		}    		
		if (n < 0)  __android_log_print(ANDROID_LOG_ERROR, "DEBUG", "[NETMANAGER]ERROR writting from socket");
		send += n;
	}
}

void NetworkManager::sendInt(int value){
	
	int send_value = htonl(value);
	send(_sockfd, (const char*)&send_value, 4, 0);
}

void NetworkManager::sendUnsignedInt(unsigned int value){
	
	unsigned int send_value = htonl(value);
	send(_sockfd, (const char*)&send_value, 4, 0);
}

int NetworkManager::receiveInt(){
	int value;
	recv(_sockfd, &value, 4, 0);
	return ntohl(value);
}

unsigned int NetworkManager::receiveUnsignedInt() {
	unsigned int my_net_id;
	unsigned int client_id;
	recv(_sockfd, &my_net_id, 4, 0);
	client_id = ntohl(my_net_id);  
	return client_id;
}

void* NetworkManager::receiveVoidBuffer(int bufferSize){
	
	char* buffer = (char*) malloc (bufferSize);
	bzero(buffer,bufferSize);
	int received = 0;
	int n;
	while(received < bufferSize){
		if(received == 0)
			n = read(_sockfd,buffer,bufferSize);
		else
			n = read(_sockfd,((char*)buffer+received),bufferSize-received);
		if (n < 0) printf("[NETMANAGER]ERROR reading from socket\n");
		received += n;
	}
	buffer[bufferSize] = 0;
	return buffer;
}

uint32_t* NetworkManager::receiveBitmap(int bitmapSize){
	uint32_t *bitmapBuffer = (uint32_t*) malloc(bitmapSize);
	int n;
	int received = 0;
	while(received < bitmapSize){
		if(received == 0)
			n = read(_sockfd,bitmapBuffer,bitmapSize);
		else
			n = read(_sockfd,bitmapBuffer+(received/sizeof(uint32_t)),bitmapSize-received);
		if (n < 0) __android_log_print(ANDROID_LOG_ERROR, "DEBUG","[NETMANAGER]ERROR reading from socket");
		received += n;
	}
	return bitmapBuffer;
}

std::string NetworkManager::receiveTagMessage(){
	size_t tagSize = 10;
	size_t msgSize = (tagSize*sizeof(char));
	char *tag = (char*) malloc (msgSize);
	tag = (char*)receiveVoidBuffer(msgSize);
	std::string str(tag);
	return str;
}

std::string NetworkManager::receiveKernelString(){
	int kernelSize = receiveInt();
	int msgSize = (kernelSize*sizeof(char));
	char *kernelString = (char*) malloc (msgSize);
	kernelString = (char*)receiveVoidBuffer(msgSize);
	std::string str(kernelString);
	return str;
}

void NetworkManager::receiveArgBuffer(KernelArgument &argsBuffers){
	std::string argKernelName;

	/* Receiving Buffer data */
	argKernelName 			= receiveKernelString();	//KernelName
	int bufferArg 			= receiveInt(); 			//[1] BufferArg [0] Primitive Arg
	unsigned int argSize 	= receiveUnsignedInt();		
	int argId 				= receiveInt();
	int empty 				= receiveInt();
	void* argBuffer;
	if(!empty) argBuffer 	= receiveVoidBuffer(argSize);//Buffer if not empty
	
	if(bufferArg) bufferArg = 1;
	else bufferArg = 0;
	
	/* Creating Buffer object */
	auto buff = std::make_shared<parallelus::Buffer>(argSize);
	if(!empty) buff->setSource(argBuffer);
	buff->setId(argId);

	argsBuffers[argKernelName].first.push_back(buff);
}

void NetworkManager::receiveWorkSize(KernelArgument &argsBuffers){
	std::string kernelNameString;

	kernelNameString = receiveKernelString();
	int xdim = receiveUnsignedInt();
	int ydim = receiveUnsignedInt();
	int zdim = receiveUnsignedInt();

	argsBuffers[kernelNameString].second = std::make_tuple(xdim,ydim,zdim);
}

void NetworkManager::sendOutputBuffer(std::shared_ptr<parallelus::Buffer> outputBuffer){
	sendString("RESULT____");
	void* cpy = (void*) malloc (outputBuffer->size());
	outputBuffer->copyTo(cpy,0);
	sendUnsignedInt(outputBuffer->size());
	sendVoidBuffers(cpy, outputBuffer->size());
}