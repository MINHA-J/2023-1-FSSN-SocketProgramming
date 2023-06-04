#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#include <thread>
#include <vector>
#include "Socket.h"
#pragma comment(lib, "ws2_32")
using namespace std;

#define PORT 65456
#define BufferSize 1024

vector<Socket> socketVector;
bool flag = true;

void ReceiveData(SOCKET clientSocket, int number)
{
	char RecvData[BufferSize] = { 0, };

	while (flag)
	{
		int recvLen = recv(clientSocket, RecvData, sizeof(RecvData), 0);
		
		if (recvLen == SOCKET_ERROR || recvLen == 0) 
			break;
		if (strcmp(RecvData, "quit") == 0)
		{
			cout << "> echoed: quit by Thread-" << number + 2 << endl;
			Socket::SubClientNumber();
			continue;
		}

		while (flag)
		{
			cout << "> received ( " << RecvData << " ) and echoed to " << Socket::GetClientNumber() - 1 << " threads" << endl;
			for (int i = 0; i < Socket::GetClientNumber() - 1; ++i)
			{
				if (send(socketVector[i].GetClientSocket(), RecvData, sizeof(RecvData), 0) == SOCKET_ERROR)
					break;
			}
			break;
		}
	}
	return;
}

void AcceptClients(SOCKET listenSocket, SOCKADDR_IN* serverAddr)
{
	int number = 0;

	while (flag)
	{
		socketVector.push_back(Socket());
		socketVector[number].SetClientSocket(accept(listenSocket, (SOCKADDR*)&socketVector[number].GetClientAddr(), &socketVector[number].GetClientSize()));

		if (socketVector[number].GetClientSocket() == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			return;
		}

		Socket::AddClientNumber();
		socketVector[number].SetNumber(number);

		char ipAddress[16];
		inet_ntop(AF_INET, &socketVector[number].GetClientAddr().sin_addr, ipAddress, sizeof(ipAddress));
		cout << "> client connected by IP address " << ipAddress << " with Port number " << ::ntohs((*serverAddr).sin_port) << endl;

		thread(ReceiveData, socketVector[number].GetClientSocket(), socketVector[number].GetNumber()).detach();

		number++;
	}

	return;
}

int main()
{
	WSADATA wsaData = { 0, }; //Socket 초기화 정보를 저장하기 위한 구조체
	SOCKET ServerSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };
	SOCKADDR_IN ClientAddr = { 0, };
	char RecvData[BufferSize] = { 0, };
	int ClientSize = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) return 0;

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (ServerSocket != INVALID_SOCKET)
	{
		try
		{
			if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
			{
				cout << "> bind() failed and program terminated" << endl;
				closesocket(ServerSocket);
				return 0;
			}
		}
		catch (exception&)
		{
			cout << "> bind() failed by exception" << endl;
			closesocket(ServerSocket);
			return 0;
		}

		if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			cout << "> listen() failed and program terminated" << endl;
			closesocket(ServerSocket);
			return 0;
		}
	}

	cout << "> echo-server is activated" << endl;
	cout << "> server loop running in thread (main thread): Thread-1" << endl;
	thread(AcceptClients, ServerSocket, &ServerAddr).detach();
	
	char command[15];
	while (flag)
	{
		cin >> command;
		if (!strncmp(command, "quit", sizeof("quit")))
		{
			if (Socket::GetClientNumber() == 1)
			{
				flag = false;
				break;
			}
			else
				cout << "> active threads are remained : " << Socket::GetClientNumber() - 1 << " threads" << endl;
		}
	}

	cout << "> echo-server is de-activated" << endl;

	closesocket(ServerSocket);
	WSACleanup();
	system("pause");


	return 0;
}