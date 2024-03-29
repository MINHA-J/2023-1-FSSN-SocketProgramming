#include <iostream>
#include <WinSock2.h> //For Socket Programming
#include <WS2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
using namespace std;

#define PORT 65456
#define BufferSize 1024

int main()
{
	WSADATA wsaData = { 0, }; //Socket 초기화 정보를 저장하기 위한 구조체
	SOCKET ServerSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };
	SOCKADDR_IN ClientAddr = { 0, };
	char RecvData[BufferSize] = { 0, };
	int ClientSize = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) return 0;

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	memset(&ClientAddr, 0, sizeof(ClientAddr));
	ClientSize = sizeof(ClientAddr);
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientSize);

	char IpAddress[16];
	inet_ntop(AF_INET, &ClientAddr.sin_addr, IpAddress, sizeof(IpAddress));
	cout << "> echo-server is activated" << endl;
	cout << "> client connected by IP address " << IpAddress << " with Port number " << ntohs(ServerAddr.sin_port) << endl;

	while (ClientSocket != INVALID_SOCKET)
	{
		memset(&RecvData, 0, sizeof(RecvData));
		recv(ClientSocket, RecvData, sizeof(RecvData), 0);
		cout << "> echoed: " << RecvData << endl;
		send(ClientSocket, RecvData, sizeof(RecvData), 0);

		if (strcmp(RecvData, "quit") == 0) break;
	}

	closesocket(ServerSocket);
	WSACleanup();

	std::cout << "> echo-server is de-activated" << std::endl;

	return 0;
}