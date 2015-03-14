#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <node.h>
#include <uv.h>
#include <sys/socket.h>
#include <sys/types.h>

/*
    A simple interface for receiving audio data via UDP.
    Implemented using standard UNIX tooling. See UdpSender for an explanation.
*/
class UdpReceiver {
	public:
		UdpReceiver(const char* ip, int port);
		int Receive(void* data, int dataLength);

	private:
	    int socketNum;
};




#endif
