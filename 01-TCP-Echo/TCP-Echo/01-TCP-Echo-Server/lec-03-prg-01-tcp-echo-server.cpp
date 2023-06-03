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
	WSADATA wsaData = { 0, }; //Socket �ʱ�ȭ ������ �����ϱ� ���� ����ü
	SOCKET ServerSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };
	SOCKADDR_IN ClientAddr = { 0, };
	char RecvData[BufferSize] = { 0, };
	int ClientSize = 0;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)); //socket�� ���� ������ �ּ� ������ ����
	listen(ServerSocket, SOMAXCONN); //Socket�� ���� ��� ���·� ����

	memset(&ClientAddr, 0, sizeof(ClientAddr));
	ClientSize = sizeof(ClientAddr);
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientSize);

	char IpAddress[16];
	inet_ntop(AF_INET, &ClientAddr.sin_addr, IpAddress, sizeof(IpAddress));
	cout << "> echo-server is activated" << endl;
	cout << "> client connected by IP address " << IpAddress << " with Port number " << ntohs(ServerAddr.sin_port) << endl;

	while (true)
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

