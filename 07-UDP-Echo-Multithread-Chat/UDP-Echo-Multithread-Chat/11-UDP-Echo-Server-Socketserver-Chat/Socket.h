#pragma once
#include <chrono>
#include <thread>
#include <atomic>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
using namespace std;

class Socket
{
private:
	SOCKET			clientSocket;
	SOCKADDR_IN		clientAddress;
	int				clientSize;

public:
	Socket(SOCKET socket, SOCKADDR_IN addr)
	{
		memset(&clientSocket, 0, sizeof(clientSocket));
		memset(&clientAddress, 0, sizeof(clientAddress));
		
		clientSocket = socket;
		clientAddress = addr;
		clientSize = sizeof(clientAddress);
	}
	~Socket()
	{
		memset(&clientSocket, 0, sizeof(clientSocket));
		memset(&clientAddress, 0, sizeof(clientAddress));
		clientSize = -1;
	}

public:
	void SetClientSocket(SOCKET socket) { clientSocket = socket; }
	void	SetClientAddr(SOCKADDR_IN addr) { clientAddress = addr; }
	void	SetClinetSize(int size) { clientSize = size; }


	SOCKADDR_IN& GetClientAddr() { return clientAddress; }
	SOCKET& GetClientSocket() { return clientSocket; }
	int& GetClientSize() { return clientSize; }
};