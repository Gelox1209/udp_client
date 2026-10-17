#define WIN32_LEAN_AND_MEAN

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <iostream>

int main(int argc, char* argv[])
{
    //Initialize Winsock
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0)
    {
        std::cout << "Error in WSAStartup: " << WSAGetLastError() << std::endl;
        return 1;
    }
    //Create UDP Socket
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        std::cout << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    //Create a remote address structure from the IP string 34.213.232.150 and port 8888
    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    memset(&destAddr.sin_zero, 0, 0);//Set Zero's
    destAddr.sin_port = htons(8888);
    const char* ip = "34.213.232.150";
    //Set port
    res = inet_pton(AF_INET, ip,  &destAddr.sin_addr);
    if (res != 1)
    {
        std::cout << "Error in inet_pton: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    //Send data over the socket
    const char* message = argv[0];
    res = sendto(udpSocket, message, (int)strlen(message), 0, (struct sockaddr*)&destAddr, (int)sizeof(destAddr));
    if (res == SOCKET_ERROR)
    {
        std::cout << "Error sending: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    //Listen for a responce
    int recvBufferLength = 1500;
    char* recvBuffer = new char[recvBufferLength];
    memset(recvBuffer, 0, recvBufferLength);

    res = recvfrom(udpSocket, recvBuffer, recvBufferLength - 1, 0, NULL, NULL);
    if (res == SOCKET_ERROR)
    {
        std::cout << "Receive error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << recvBuffer << std::endl;//Print contents of buffer to screen
    delete[] recvBuffer;
    //Close the socket
    res = closesocket(udpSocket);
    if (res != 0)
    {
        std::cout << "Error closing socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    //Shutdown Winsock
    WSACleanup();
}
