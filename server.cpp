//Server

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <cstdlib>
using namespace std;

SOCKET connections[100];
int connectionCounter = 0;

enum Packet {
	P_chatMessage, 
	P_test
};

bool processPacket(int ID, Packet packettype) {
	switch (packettype) {
		case P_chatMessage: {
			int bufferlength;
			recv(connections[ID], (char*)& bufferlength, sizeof(int), NULL);
			char * buffer = new char[bufferlength];
			recv(connections[ID], buffer, bufferlength, NULL);

			for (int i = 0; i < connectionCounter; i++) {
				if (i == ID) {
					continue;
				}
				Packet chatmessagepacket = P_chatMessage;
				send(connections[i], (char*)&chatmessagepacket, sizeof(Packet), NULL);
				send(connections[i], (char*)&bufferlength, sizeof(int), NULL);
				send(connections[i], buffer, bufferlength, NULL);
			}
			delete[] buffer;
			break;
		}
		default:
			cout << "Unrecognized packet: " << packettype << endl;
			break;
		
	}
}

int clientHandlerThread(int ID, Packet packettype) {
	while (true) {
		recv(connections[ID], (char*)& packettype, sizeof(Packet), NULL);

		if (!processPacket(ID, packettype)) {
			break;
		}


	}
	closesocket(connections[ID]);
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
	int addrlen = sizeof(addr); //length of address required to accept call
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // broadcast locally
	addr.sin_port = htons(1111); //port
	addr.sin_family = AF_INET; //ipv4 socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //create socket to listen 
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // bind the address to the socket
	listen(sListen, SOMAXCONN); //put sListen in socket to listen for incoming 

	SOCKET newConnection; //socket to hold clients connection
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); // accept the connection
		if (newConnection == 0) {
			cout << "failed to accept client's connection.  " << endl;
		}
		else {
			connections[i] = newConnection;
			connectionCounter++;
			cout << "client connected!" << endl;
			string message = "Welcome! This is a message.  ";
			int msize = sizeof(message);
			//send(newConnection, (char*) & msize, sizeof(int));
			//send(newConnection, message.c_str(), sizeof(message), NULL);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandlerThread, (LPVOID)(i), NULL, NULL);
			Packet chatmessagepacket = P_chatMessage;
			send(newConnection, (char*)&chatmessagepacket, sizeof(Packet), NULL);
			string buftest = message;
			int bsize = sizeof(buftest);
			send(newConnection, (char*)&bsize, sizeof(int), NULL);
			send(newConnection, (char*)&buftest, bsize, NULL);
			Packet testpacket = P_test;
			send(newConnection, (char*)&testpacket, sizeof(Packet), NULL);
		}
	}


	system("PAUSE");
	return 0;

}
