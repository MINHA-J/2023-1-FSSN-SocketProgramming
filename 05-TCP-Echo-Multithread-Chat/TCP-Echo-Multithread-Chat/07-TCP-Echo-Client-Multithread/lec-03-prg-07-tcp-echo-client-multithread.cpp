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

void SendData(SOCKET clientSocket)
{
	char SendData[BufferSize] = { 0, };
	while (IsActive)
	{
		cout << "> "; 
		cin >> SendData;

		if (send(clientSocket, SendData, sizeof(SendData), 0) == SOCKET_ERROR)
			break;

		if (strcmp(SendData, "quit") == 0)
		{
			IsActive = false;
			break;
		}
			
	}
}

void ReceiveData(SOCKET clientSocket)
{
	char RecvData[BufferSize] = { 0, };
	while (IsActive)
	{
		int recvLen = recv(clientSocket, RecvData, sizeof(RecvData), 0);
		if (recvLen == SOCKET_ERROR || recvLen == 0)
			break;

		cout << "> received: " << RecvData << endl;
	}
}

int main()
{
	WSADATA wsaData = { 0, }; //Socket 초기화 정보를 저장하기 위한 구조체
	SOCKET ClientSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };
	

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) return 0;

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) return 0;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, HOST, &ServerAddr.sin_addr);

	if (ClientSocket != INVALID_SOCKET)
	{
		try
		{
			if (connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
			{
				cout << "> connect() failed and program terminated" << endl;
				closesocket(ClientSocket);
				return 0;
			}
		}
		catch (exception&)
		{
			cout << "> connect() failed by exception" << endl;
			closesocket(ClientSocket);
			return 0;
		}
	}

	cout << "> echo-client is activated" << endl;
	thread(SendData, ClientSocket).detach();
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