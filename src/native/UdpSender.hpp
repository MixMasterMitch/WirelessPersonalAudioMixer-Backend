#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include <node.h>
#include <uv.h>
#include <sys/socket.h>
#include <sys/types.h>

/*
    A simple interface for sending audio data to an ip and port via UDP
    Implemented using the standard UNIX tooling. I attempted to use lib uv (the node way), but the overhead of
    having the packets queued in the event loop was a problem.
*/
class UdpSender {
	public:
		UdpSender();
		void Send(const char* ip, int port, void* data, int length);

	private:
//		uv_udp_t sendSocket;
//		uv_loop_t* loop;
		int socketNum;
//		static void onSend(uv_udp_send_t* req, int status) {
//			// printf("Message Sent %d\n", status);
//		}
};




#endif
