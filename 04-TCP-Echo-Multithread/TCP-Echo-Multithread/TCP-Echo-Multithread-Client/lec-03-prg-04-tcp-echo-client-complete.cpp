#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
using namespace std;

#define HOST "127.0.0.1"
#define PORT 65456
#define BufferSize 1024

int main()
{
	WSADATA wsaData = { 0, }; //Socket 초기화 정보를 저장하기 위한 구조체
	SOCKET ClientSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };
	char RecvData[BufferSize] = { 0, };

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) return 0;

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	char SendMsg[BufferSize] = {};
	while (true)
	{
		cout << "> ";
		cin >> SendMsg;

		send(ClientSocket, SendMsg, sizeof(SendMsg), 0);
		if (strcmp(SendMsg, "quit") == 0) break;
		recv(ClientSocket, RecvData, sizeof(RecvData), 0);
		cout << "> received: " << RecvData << endl;
	}

	cout << "> echo-client is de-activated" << endl;

	closesocket(ClientSocket);
	WSACleanup();
	system("pause");

	

	return 0;
}