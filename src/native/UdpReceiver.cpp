#include "UdpReceiver.hpp"

UdpReceiver::UdpReceiver(const char* ip, int port) {
    socketNum = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in receiverAddress;
    receiverAddress.sin_family = AF_INET;
    receiverAddress.sin_port = htons(port);
    receiverAddress.sin_addr.s_addr = inet_addr(ip);

    bind(socketNum, (struct sockaddr*) &receiverAddress, sizeof(receiverAddress));
}

int UdpReceiver::Receive(void* data, int dataLength) {
    struct sockaddr_in senderAddress;
    socklen_t senderAddressLength = sizeof(senderAddress);
    return recvfrom(socketNum, data, dataLength, 0, (struct sockaddr*) &senderAddress, &senderAddressLength);
}
