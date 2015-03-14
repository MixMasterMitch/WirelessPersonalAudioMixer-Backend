#include "UdpSender.hpp"

UdpSender::UdpSender() {
    socketNum = socket(AF_INET, SOCK_DGRAM, 0);

//	loop = uv_loop_new();
//	// loop = uv_default_loop();
//	uv_udp_init(loop, &sendSocket);
//    struct sockaddr_in bindAddr;
//    int error = uv_ip4_addr("0.0.0.0", 0, &bindAddr);
//    if (error) printf("ERROR: Could not create UDP server bind address.");
//    error = uv_udp_bind(&sendSocket, (const struct sockaddr *)&bindAddr, 0);
//    if (error) printf("ERROR: Could not bind UDP server port.");
}

void UdpSender::Send(const char* ip, int port, void* data, int length) {
    struct sockaddr_in receiverAddress;
    receiverAddress.sin_family = AF_INET;
    receiverAddress.sin_port = htons(port);
    receiverAddress.sin_addr.s_addr = inet_addr(ip);

    sendto(socketNum, data, length, 0, (struct sockaddr*) &receiverAddress, sizeof(receiverAddress));

//    uv_udp_send_t req;
//    uv_buf_t msg = uv_buf_init((char*) data, length);
//
//    struct sockaddr_in addr;
//    int error = uv_ip4_addr(ip, port, &addr);
//    if (error) printf("ERROR: Could not create UDP server send address.");
//    error = uv_udp_send(&req, &sendSocket, &msg, 1, (const struct sockaddr*) &addr, UdpSender::onSend);
//    if (error) printf("ERROR: Could not send UDP message.");
//
//    uv_run(loop, UV_RUN_DEFAULT);
}
