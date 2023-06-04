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

USHORT PortNum = 0;
bool findPortValue(pair<USHORT, Socket> a)
{
	if (a.first == PortNum)
		return true;
	return false;
}


int main()
{
	
	WSADATA wsaData = { 0, }; //Socket 초기화 정보를 저장하기 위한 구조체
	SOCKET ServerSocket = NULL;
	SOCKADDR_IN ServerAddr = { 0, };
	SOCKADDR_IN ClientAddr = { 0, };
	char RecvData[BufferSize] = { 0, };
	int ClientSize = 0;
	vector<pair<USHORT, Socket>> Sockets;

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

	Sockets.clear();
	while (true)
	{
		memset(&ClientAddr, 0, sizeof(ClientAddr));
		int Length = sizeof(ClientAddr);
		int recvLen = recvfrom(ServerSocket, RecvData, sizeof(RecvData), 0, (SOCKADDR*)&ClientAddr, &Length);
		
		if (recvLen <= 0)
			return 0;

		if (strcmp(RecvData, "quit") == 0)
		{
			PortNum = ClientAddr.sin_port;
			auto it = find_if(Sockets.begin(), Sockets.end(), findPortValue);
			if (it != Sockets.end())
			{
				Sockets.erase(it);
				continue;
			}
			continue;
		}
		else if (strcmp(RecvData, "#REG") == 0)
		{
			PortNum = ClientAddr.sin_port;
			auto it = find_if(Sockets.begin(), Sockets.end(), findPortValue);
			if (it == Sockets.end())
			{
				Socket socket = Socket(ServerSocket, ClientAddr);
				Sockets.push_back(make_pair(ClientAddr.sin_port, socket));

				char ipAddress[16];
				inet_ntop(AF_INET, &socket.GetClientAddr().sin_addr, ipAddress, sizeof(ipAddress));
				cout << "> client registered ('" << ipAddress << "', " << ntohs(ServerAddr.sin_port) << ")" << endl;
				continue;
			}
		}
		else if (strcmp(RecvData, "#DEREG") == 0)
		{
			PortNum = ClientAddr.sin_port;
			auto it = find_if(Sockets.begin(), Sockets.end(), findPortValue);
			if (it != Sockets.end())
			{
				char ipAddress[16];
				inet_ntop(AF_INET, &(*it).second.GetClientAddr().sin_addr, ipAddress, sizeof(ipAddress));
				cout << "> client de-registered ('" << ipAddress << "', " << ntohs(ServerAddr.sin_port) << ")" << endl;
				it = Sockets.erase(it);
				continue;
			}
		}

		if (Sockets.begin() == Sockets.end())
		{
			cout << "> no clients to echo" << endl;
			continue;
		}

		for (auto iter = Sockets.begin(); iter != Sockets.end(); ++iter)
		{
			int number = 0;
			int errorCode = sendto(iter->second.GetClientSocket(), RecvData, recvLen, 0,
				(SOCKADDR*)&iter->second.GetClientAddr(), sizeof(iter->second.GetClientAddr()));
			
			if (errorCode == SOCKET_ERROR)
				return 0;
			
		}
		cout << "> received ( " << RecvData << " ) and echoed to " << Sockets.size() << " clients" << endl;

	}

	cout << "> echo-server is de-activated" << endl;
	closesocket(ServerSocket);
	WSACleanup();
	system("pause");

	return 0;
}