//Client

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <cstdlib>



using namespace std;
SOCKET connection;

void ClientThread() {
	char buffer[256];
	while (true) {
		recv(connection, buffer, sizeof(buffer), NULL);
		cout << buffer << endl;
	}
}

int main() {
	//winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr; //address we will bind our listenting socket to
	int sizeofaddr = sizeof(addr); //length of address required to accept call
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // broadcast locally
	addr.sin_port = htons(1111); //port
	addr.sin_family = AF_INET; //ipv4 socket

	connection = socket(AF_INET, SOCK_STREAM, NULL); // set connection socket
	if (connect(connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		MessageBoxA(NULL, "Failed to connect", "ERROR", MB_OK | MB_ICONERROR);
		return 0; // failed to connect
	}
	
	cout << "Connected!" << endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

	char buffer[256];

	while (true) {
		cin.getline(buffer, sizeof(buffer));
		send(connection, buffer, sizeof(buffer), NULL);
		Sleep(10);
	}

	system("PAUSE");
	return 0;
}