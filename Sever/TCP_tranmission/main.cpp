#include<iostream>
#include<WS2tcpip.h>
#include<string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

void main() 
{
	//initialze winsock
	WSADATA wsdata;
	WORD ver = MAKEWORD(2, 2);
	
	int wsOK = WSAStartup(ver, &wsdata);
	if (wsOK != 0) 
	{
		cerr << "Can't initialze winsock" << endl;
		return;
	}
	while (true)
	{
		SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in client;

		if (listening == INVALID_SOCKET)
		{
			cerr << "Can't create a socket" << endl;
			return;
		}

		//set parameters
		int port;
		cout << "Nhap port: ";
		cin >> port;
		cout << endl;
		string add;
		cout << "Nhap dia chi: ";
		cin >> add;
		string userInput;
		string check;
		cout << "nhap trang thai(send/recv): ";
		cin >> check;

		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listening, (sockaddr*)&hint, sizeof(hint));

		// Tell Winsock the socket is for listening 
		listen(listening, SOMAXCONN);

		//wait for a connection

		int clientsize = sizeof(client);

		SOCKET clientsocket;
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];



		for (int i = 0;i < add.length();i++)
		{
			if (add.at(i) == '.') add.at(i) = ' ';
		}
		istringstream is(add);
		int n, i = 0, a[4];
		while (is >> n) {
			a[i++] = n;
		}
		client.sin_addr.S_un.S_un_b.s_b1 = a[0];
		client.sin_addr.S_un.S_un_b.s_b2 = a[1];
		client.sin_addr.S_un.S_un_b.s_b3 = a[2];
		client.sin_addr.S_un.S_un_b.s_b4 = a[3];

		clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			cout << host << "connected on port" << service << endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << "connected on port" <<
				ntohs(client.sin_port) << endl;
		}

		//close sever
		closesocket(listening);
		send(clientsocket, "quan", 4 + 1, 0);
		char buf[4096];

		ZeroMemory(buf, 4096);
		if (check == "send") {
			//wait for client send data
			cout << string("Ready", 0, 5) << endl;
			//echo message back to client
			int bytesReceived = recv(clientsocket, buf, 4096, 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				cerr << "GUi that bai" << endl;
			}
			if (bytesReceived == 0)
			{
				cout << "Client disconnect" << endl;
			}
			send(clientsocket, "quan", 4 + 1, 0);



			cout << string("Gui thanh cong", 0, 14) << endl;
		}
		else
		{
			//wait for client send data
			int bytesReceived = recv(clientsocket, buf, 4096, 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				cerr << "Error in recv(). Quitting.." << endl;
				return;
			}
			if (bytesReceived == 0)
			{
				cout << "Client disconnect" << endl;
				return;
			}
			//cout << string("Ready", 0, 5) << endl;
			//echo message back to client
			send(clientsocket, "ok", 2 + 1, 0);
			cout << string(buf, 0, bytesReceived) << endl;
			cout << string("Nhan thanh cong", 0, 15) << endl;
		}
		closesocket(clientsocket);
	}
	//while (true) 
	//{
	//	ZeroMemory(buf, 4096);

	//	//wait for client send data
	//	int bytesReceived = recv(clientsocket, buf, 4096, 0);
	//	if (bytesReceived == SOCKET_ERROR) 
	//	{
	//		cerr << "Error in recv(). Quitting.." << endl;
	//		break;
	//	}
	//	if (bytesReceived == 0) 
	//	{
	//		cout << "Client disconnect" << endl;
	//		break;
	//	}
	//	cout << string(buf, 0, bytesReceived) << endl;
	//	//echo message back to client
	//	send(clientsocket, buf, bytesReceived + 1,0);
	//}
	//close the socket
	/*closesocket(clientsocket);*/
	//cleanup winsock
	WSACleanup();

}