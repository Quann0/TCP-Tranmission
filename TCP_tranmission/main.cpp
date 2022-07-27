#include<iostream>
#include<WS2tcpip.h>
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

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket" << endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	//wait for a connection
	sockaddr_in client;
	int clientsize = sizeof(client);
	
	SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	
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
	
	char buf[4096];
	while (true) 
	{
		ZeroMemory(buf, 4096);

		//wait for client send data
		int bytesReceived = recv(clientsocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) 
		{
			cerr << "Error in recv(). Quitting.." << endl;
			break;
		}
		if (bytesReceived == 0) 
		{
			cout << "Client disconnect" << endl;
			break;
		}
		cout << string(buf, 0, bytesReceived) << endl;
		//echo message back to client
		send(clientsocket, buf, bytesReceived + 1,0);
	}
	//close the socket
	closesocket(clientsocket);
	//cleanup winsock
	WSACleanup();

}