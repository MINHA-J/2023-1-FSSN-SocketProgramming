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
	int				number;
private:
	static atomic<int> clientNumber;

public:
	Socket()
	{
		memset(&clientSocket, 0, sizeof(clientSocket));
		memset(&clientAddress, 0, sizeof(clientAddress));
		clientSize = sizeof(clientAddress);
		number = 0;
	}
	~Socket()
	{
		memset(&clientSocket, 0, sizeof(clientSocket));
		memset(&clientAddress, 0, sizeof(clientAddress));
		clientSize = -1;
		number = -1;
	}

public:
	void SetClientSocket(SOCKET socket) { clientSocket = socket; }
	void SetNumber(int num) { number = num; }

	SOCKADDR_IN& GetClientAddr() { return clientAddress; }
	SOCKET& GetClientSocket() { return clientSocket; }
	int& GetClientSize() { return clientSize; }
	int& GetNumber() { return number; }

public:
	static int	GetClientNumber() { return clientNumber; }
	static void		AddClientNumber() { clientNumber.fetch_add(1); }
	static void		SubClientNumber() { clientNumber.fetch_add(-1); }

};

atomic<int> Socket::clientNumber = ATOMIC_VAR_INIT(1);