//Client

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;
SOCKET connection;

enum Packet {
	P_chatMessage,
	P_test
};

bool processPacket(Packet packettype) {
	switch (packettype){
	case P_chatMessage:
	{
		int bufferlength;
		recv(connection, (char*)&bufferlength, sizeof(int), NULL);
		char * buffer = new char[bufferlength + 1];
		buffer[bufferlength] = '\0';
		recv(connection, buffer, bufferlength, NULL);
		cout << ">" << buffer << endl;
		delete[] buffer;
		break;
	}
	case P_test:
		cout << "you have recieved the test package...nice " << endl;
		break;
	default: 
		cout << "Unrecognized Packet: " << packettype << endl;
		break;
	}
}

void ClientThread() {
	while (true) {
		Packet packettype;
		recv(connection, (char*)&packettype, sizeof(Packet), NULL);
		if (!processPacket(packettype)) {
			break;
		}

	}
	closesocket(connection);

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

	string buffer;

	while (true) {
		getline(cin, buffer);
		int bufferlength = buffer.size();
		Packet packettype;
		send(connection, (char*)&packettype, sizeof(Packet), NULL);
		send(connection, (char*)&bufferlength, sizeof(int), NULL);
		send(connection, buffer.c_str(), bufferlength, NULL);

		Sleep(10);
	}

	system("PAUSE");
	return 0;
}