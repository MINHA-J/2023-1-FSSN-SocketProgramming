#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#include <thread>
#include <vector>
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

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	ServerSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (ServerSocket == INVALID_SOCKET) 
		return 0;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "> bind() failed and program terminated" << endl;
		return 0;
	}
	cout << "> echo-server is activated" << endl;

	while (true)
	{
		memset(&ClientAddr, 0, sizeof(ClientAddr));

		int Length = sizeof(ClientAddr);
		int recvLen = recvfrom(ServerSocket, RecvData, sizeof(RecvData), 0, (SOCKADDR*)&ClientAddr, &Length);
		if (recvLen <= 0)
			return 0;

		int errorCode = sendto(ServerSocket, RecvData, recvLen, 0, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr));
		if (errorCode == SOCKET_ERROR)
			return 0;

		cout << "> echoed: " << RecvData << endl;
	}

	cout << "> echo-server is de-activated" << endl;

	closesocket(ServerSocket);
	WSACleanup();
	system("pause");

	return 0;

}