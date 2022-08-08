#include<iostream>
#include<stdio.h>
#include<WS2tcpip.h>
#include<string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <memory>
#include <vector>
#include <synchapi.h>
#pragma comment (lib,"ws2_32.lib")
#define maxsize 1500000000
using namespace std;


struct Settings
{
	string filename;
	string filesizerecv;
	string pathfile;
	string textfile;
	string add;
	int portnumber = 0;
	char numtext[11] = {0};
};

void main() 
{
	/*ifstream file("E:\\text\\dragonica.exe_20220516004454.dmp", ios::in | ios::binary );
	ofstream MyOutFile("E:\\text\\data\\dragonica.exe_20220516004454.dmp", ios::in | ios::binary );
	MyOutFile << file.rdbuf();
	file.close();
	MyOutFile.close();*/
	Settings st;
	st.filename = "";
	string check;
	int bytesReceived;
	int sendResult;
	int numtext,checknum = 0;
	int sizefile;
	unsigned int size,datafilenumber = 0;
	char* memblock;
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

		bytesReceived = 0;
		sendResult = 0;
		numtext = 0;
		sizefile = 0;
		datafilenumber = 0;
		st.portnumber = 0;
		st.add = "";
		check = "";
		
		ZeroMemory(st.numtext, 11);
		cout << "Portnumber: ";
		cin >> st.portnumber;
		cout << endl;

		cout << "Nhap dia chi: ";
		cin >> st.add;
		
		cout << "Mode(send/recv): ";
		cin >> check;
		

		
		
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(st.portnumber);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listening, (sockaddr*)&hint, sizeof(hint));

		// Tell Winsock the socket is for listening 
		listen(listening, SOMAXCONN);

		//wait for a connection

		int clientsize = sizeof(client);

		SOCKET clientsocket;
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];



		for (int i = 0;i < st.add.length();i++)
		{
			if (st.add.at(i) == '.') st.add.at(i) = ' ';
		}
		istringstream is(st.add);
		int n, i = 0, a[4] = {0};
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
			cout << host << "connected on st.portnumber" << service << endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << "connected on st.portnumber" <<
				ntohs(client.sin_port) << endl;
		}

		//close sever
		closesocket(listening);
		//send(clientsocket, "quan", 4 + 1, 0);
		char buf[4096];
		getchar();
		ZeroMemory(buf, 4096);
		if (check == "send") {
			
				cout << "Nhap path file send: ";
				getline(cin, st.pathfile);

				for (int i = st.pathfile.length() - 1;i >= 0;i--)
				{
					if (st.pathfile.at(i) == '\\')
					{
						st.filename = st.pathfile.substr(i + 1);
						break;
					}
				}

				ifstream file(st.pathfile, ios::in | ios::binary | ios::ate);
				size = (unsigned int)file.tellg();


			

			send(clientsocket, "send", 4 + 1, 0);
			
			//wait for client send data
			cout << string("Ready", 0, 5) << endl;
			//echo message back to client
			bytesReceived = recv(clientsocket, buf, 4096, 0);
			 // tin hieu da ket noi
			send(clientsocket, st.filename.c_str(), st.filename.length(), 0);

			if (bytesReceived == SOCKET_ERROR)
			{
				cerr << "GUi that bai" << endl;

			}
			if (bytesReceived == 0)
			{
				cout << "Client disconnect" << endl;
			}
			//
			strcpy_s(st.numtext,11, to_string(size).c_str());
			//st.numtext = to_string(size).c_str();
			//sprintf_s(st.numtext, "%d", (unsigned int)size);
			//st.numtext = to_string((int)size).c_str();
			send(clientsocket, st.numtext, 11, 0);

			
			do
			{
				if (size > 0)
				{

					if (datafilenumber >= size) break;
					if (((unsigned int)size - datafilenumber) < maxsize)
					{
						memblock = new char[((unsigned int)size - datafilenumber)];
						file.seekg(0, ios::beg);
						file.read(memblock + datafilenumber, (datafilenumber - (unsigned int)size));
						do
						{
							if (size - numtext < 4096)
							{
								//send(clientsocket, memblock + numtext, (maxsize - numtext), 0);
								send(clientsocket, memblock + numtext, (size - numtext), 0);
							}
							else
							{
								send(clientsocket, memblock + numtext, 4096, 0);
							}
							numtext += 4096;
						} while (numtext < ((unsigned int)size - datafilenumber));
						//send(clientsocket, memblock + numtext, 4096, 0);
					}
					else
					{
						memblock = new char[maxsize];
						file.seekg(datafilenumber, ios::beg);
						file.read(memblock + datafilenumber, maxsize);
						do
						{
							if (maxsize - numtext < 4096)
							{
								send(clientsocket, memblock + numtext, (maxsize - numtext), 0);
							}
							else
							{
								send(clientsocket, memblock + numtext, 4096, 0);
							}
							numtext += 4096;
						} while (numtext < maxsize);
					}
					
					checknum += numtext;
					numtext = 0;
					datafilenumber += maxsize;
					delete[] memblock;
				}
				
			} while (1);

			if (checknum >= (int)size)
				cout << string("Gui thanh cong", 0, 14) << endl;
			else 
			{
				closesocket(clientsocket);
				continue;
			}
			checknum = 0;
			file.close();
			
		}
		else if(check == "recv")
		{
			
			send(clientsocket, "recv", 4 + 1, 0);
			
			ZeroMemory(buf, 4096);
			cout << "Nhap path file recv: ";
			cin >> st.pathfile;
			getchar();
			cout << "CLIENT(Send)> ";
			// Prompt the user for some text

			sendResult = send(clientsocket, "ok", 2 + 1, 0);

			//file name
			recv(clientsocket, buf, 4096, 0);
			st.filename = string(buf, 4096);
			st.pathfile += "\\" + string(buf, 4096);
			ofstream MyOutFile(st.pathfile, ios::binary);
			ZeroMemory(buf, 4096);
			if (sendResult != SOCKET_ERROR)
			{
				bytesReceived = recv(clientsocket, st.numtext, 10 + sizeof(char), 0);
				int numx = 0;
				int x = atoi(st.numtext);
				// Wait for response
				
				if (bytesReceived > 0)
				{
					do
					{
						if (x - numx < 4096)
						{
							recv(clientsocket, buf, x - numx, 0);
							MyOutFile << string(buf, x - numx);
						}
						else
						{
							recv(clientsocket, buf, 4096, 0);
							MyOutFile << string(buf, 4096);
						}
						numx += 4096;
						ZeroMemory(buf, 4096);
					} while (numx < x);

					//							MyOutFile << textfile;
											// Echo response to console

					ifstream file(st.pathfile, ios::in | ios::binary | ios::ate);
					size = file.tellg();
					file.close();
					if ((int)size)
					{
						cout << "nhan thanh cong" << endl;
						cout << "Ten file: " << st.filename << endl;
						cout << "Dung luong nhan duoc: " << (x / (int)size) * 100 << "%" << endl;
					}
					else
					{
						cerr << "Err #" << WSAGetLastError() << endl;
						MyOutFile.close();
						closesocket(clientsocket);
						continue;
					}
					st.pathfile = "";
					numx = 0;
					ZeroMemory(st.numtext, 11);
				}
			}
			// Close the file
			
			MyOutFile.close();
			// Close the file
			
		}
		// Close the file
		closesocket(clientsocket);
	}

	WSACleanup();

}