#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
using namespace std;
using int32 = __int32;

#define HOST "127.0.0.1"
//#define HOST "192.168.137.1"
#define PORT 65456
#define BufferSize 1024

int main()
{
	WSADATA wsaData = { 0, }; //Socket �ʱ�ȭ ������ �����ϱ� ���� ����ü
	SOCKET ClientSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, HOST, &ServerAddr.sin_addr);
	connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

	cout << "> echo-client is activated" << endl;

	while (true)
	{
		/*char sendMsg[BufferSize];
		cout << "> ";
		cin >> sendMsg;
		send(ClientSocket, sendMsg, sizeof(sendMsg), 0);

		char recvData[BufferSize];
		recv(ClientSocket, recvData, BufferSize, 0);
		cout << "> received: : " << recvData << endl;

		if (strcmp(sendMsg, "quit") == 0)
			break;*/
		char sendMsg[100];
		cout << "> ";
		cin >> sendMsg;
		int32 resultCode = ::send(ClientSocket, sendMsg, sizeof(sendMsg), 0);
		char recvData[1000];
		int32 recvLen = ::recv(ClientSocket, recvData, sizeof(recvData), 0);
		cout << "> received: " << recvData << endl;
		if (!strncmp(sendMsg, "quit", sizeof("quit"))) break;
	}

	closesocket(ClientSocket);
	WSACleanup();

	cout << "> echo-client is de-activated" << endl;

	return 0;
}