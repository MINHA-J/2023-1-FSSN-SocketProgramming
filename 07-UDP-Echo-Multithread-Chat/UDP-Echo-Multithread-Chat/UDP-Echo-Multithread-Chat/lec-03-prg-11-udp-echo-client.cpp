#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <thread>
#pragma comment(lib, "ws2_32")
using namespace std;

#define HOST "127.0.0.1"
#define PORT 65456
#define BufferSize 1024

bool IsActive = true;

void SendData(SOCKET ClientSocket, SOCKADDR_IN ServerAddr)
{
	char SendData[BufferSize] = { 0, };
	while (IsActive)
	{
		cout << "> ";
		cin >> SendData;

		int resultCode = sendto(ClientSocket, SendData, sizeof(SendData), 0, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
		if (resultCode == SOCKET_ERROR)
			return;
		if (strcmp(SendData, "quit") == 0)
		{
			IsActive = false;
			break;
		}
	}
}

void ReceiveData(SOCKET ClientSocket)
{
	char RecvData[BufferSize] = { 0, };
	SOCKADDR_IN RecvAddr = { 0, };
	while (IsActive)
	{
		int addrLen = sizeof(RecvData);
		int recvLen = recvfrom(ClientSocket, RecvData, sizeof(RecvData), 0, (SOCKADDR*)&RecvAddr, &addrLen);
		if (recvLen <= 0)
			return;
		cout << "> received: " << RecvData << endl;
	}
}

int main()
{
	WSADATA wsaData = { 0, }; //Socket 초기화 정보를 저장하기 위한 구조체
	SOCKET ClientSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) 
		return 0;

	ClientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (ClientSocket == INVALID_SOCKET) 
		return 0;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, HOST, &ServerAddr.sin_addr);

	connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	cout << "> echo-client is activated" << endl;

	thread(SendData, ClientSocket, ServerAddr).detach();
	thread(ReceiveData, ClientSocket).detach();

	while (IsActive)
	{

	}

	cout << "> echo-client is de-activated" << endl;

	closesocket(ClientSocket);
	WSACleanup();
	system("pause");

	return 0;
}