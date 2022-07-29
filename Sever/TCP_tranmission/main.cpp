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
		int bytesReceived;

		string myTextfile;
		string pathfile;
		string Textfile;
		char* temptextfile;

		string textbuffer;

		string temptext;
		
		char numtext1[10+sizeof(char)];

		int numtext = 0;

		cout << "Nhap path file send: ";
		cin >> pathfile;
		ifstream MyReadFile(pathfile);
		//ofstream MyWriteFile("C:\\Users\\QUAN\\Desktop\\monhoc\\Nhom4\\text.cpp");
		while (getline(MyReadFile, myTextfile)) {
			//// Output the text from the file
			//cout << myTextfile;
			Textfile += myTextfile;
		}
		
		temptextfile = &Textfile[0];
		sprintf_s(numtext1, "%d", Textfile.length());
		//MyWriteFile.close();

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
		//send(clientsocket, "quan", 4 + 1, 0);
		char buf[4096];

		ZeroMemory(buf, 4096);
		//ZeroMemory(temptext, 4096);
		if (check == "send") {
			send(clientsocket, "send", 4 + 1, 0);
			//wait for client send data
			cout << string("Ready", 0, 5) << endl;
			//echo message back to client
			bytesReceived = recv(clientsocket, buf, 4096, 0);
			 // tin hieu da ket noi
			if (bytesReceived == SOCKET_ERROR)
			{
				cerr << "GUi that bai" << endl;
			}
			if (bytesReceived == 0)
			{
				cout << "Client disconnect" << endl;
			}
			send(clientsocket, numtext1, 11, 0);
			//numtext = Textfile.length();
			while (numtext < Textfile.length())
			{
				temptext = Textfile.substr(numtext, 4096);
				//strncpy_s(temptext, temptextfile + numtext,4096);
				send(clientsocket, temptext.c_str(), 4096, 0);
				//ZeroMemory(temptext, 4096);
				numtext += 4096;

			}
			//cout << "SEVER(Send)>" << string(buf, 0, bytesReceived);
			//cout << string(buf, 0, bytesReceived) << endl;

			cout << string("Gui thanh cong", 0, 14) << endl;
		}
		else if(check == "recv")
		{
			send(clientsocket, "recv", 4 + 1, 0);

			//wait for client send data
			bytesReceived = recv(clientsocket, buf, 4096, 0); // tin hieu da ket noi
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
			//echo message back to client
			send(clientsocket, buf, bytesReceived + 1, 0);//gia tri nhan ve

			cout <<"CLIENT(Send)>"<< string(buf, 0, bytesReceived) << endl;//xuat gia tri nhan ve
			cout << "Status: Nhan thanh cong" << endl;
		}
		// Close the file
		MyReadFile.close();
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