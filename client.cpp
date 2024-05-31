#include<iostream>
#include<WinSock2.h> 
#include<WS2tcpip.h>
#include<string>
#include<thread>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

/*
	1-initialize winsock
	2-create socket
	3-connect to server
	4-send/ recv
	5-close socket


*/
bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;

}
void SendMsg(SOCKET s) {
	cout << "Enter your Name" << endl;
	string name;
	getline(cin, name);
	string message;
	while (1) {
		getline(cin, message);
		string msg = name + " : " + message;
		int bytesent = send(s, msg.c_str(), msg.length(), 0);
		if (bytesent == SOCKET_ERROR) {
			cout << "error in message send" << endl;
			break;
		}
		if (message == "quit") {
			cout << "stop sending message" << endl;
			break;
		}
	}
	closesocket(s);
	WSACleanup();

}
void ReceiveMsg(SOCKET s) {
	char buffer[4096];
	int recvlength;
	while (1) {
		recvlength = recv(s, buffer, sizeof(buffer), 0);
		if (recvlength <= 0) {
			cout << "disconnected from the server" << endl;
			break;
		}
	}
	closesocket(s);
	WSACleanup();
}


int main() {
	cout << "client Started" << endl;
	if (!Initialize()) {
		cout << "initialize winsock failed" << endl;
		return 1;
	}
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "invalid socket create" << endl;
	}
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if (inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr) <= 0) {
		perror("invalied IP address");
		exit(1);
	}
	if (connect(s, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		perror("Failed to connect to server");
		exit(1);
	}

	cout << "succesfully connect with server" << endl;
	thread senderthread(SendMsg, s);
	thread receiver(ReceiveMsg, s);
	senderthread.join();
	receiver.join();

	return 0;
}