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
#pragma comment (lib, "ws2_32.lib")

using namespace std;

string gethostip();
struct Settings
{
	string hostip;
	string filename;
	string filetext;
	string filesize;
	string filesizerecv;
	string pathfile;
	string textfile;
	ostringstream ss;
	int numtext = 0;
	char numtext1[11] = { 0 };
};



void main()
{
	Settings st;
	cout << gethostip() << endl;
	string ipAddress = gethostip();			// IP Address of the client
	cout << "Port Number: ";
	int port;

	cin >> port;						// Listening port # on the server
	getchar();
	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}



	// Do-while loop to send and receive data
	char buf[4096];
	int connResult;
	int bytesReceived = 0;
	int sendResult;
	char* memblock;
	streampos size;
	do
	{
		// Create socket
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 6);
		if (sock == INVALID_SOCKET)
		{
			cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
			//WSACleanup();
			return;
		}

		// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		// Connect to server
		do
		{
			connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

		} while (connResult == SOCKET_ERROR);

		ZeroMemory(buf, 4096);
		bytesReceived = 0;

		while (bytesReceived == 0)
		{
			bytesReceived = recv(sock, buf, 4096, 0);


		}
		if (strcmp(string(buf, 0, bytesReceived).c_str(), "send") == 0)
		{
			
			ZeroMemory(buf, 4096);
			cout << "Nhap path file recv: ";
			cin >> st.pathfile;
			getchar();
			cout << "SERVER(Send)> ";
			// Prompt the user for some text
				// Send the text
			sendResult = send(sock, "ok", 2 + 1, 0);

			//file name
			recv(sock, buf, 4096, 0);
			st.filename = string(buf, 4096);
			st.pathfile += "\\" + string(buf, 4096);
			ofstream MyOutFile(st.pathfile, ios::binary);
			ZeroMemory(buf, 4096);
			string textfile;
			if (sendResult != SOCKET_ERROR)
			{
				bytesReceived = recv(sock, st.numtext1, 10 + sizeof(char), 0);
				int numx = 0;
				int x = atoi(st.numtext1);
				// Wait for response
				ZeroMemory(st.numtext1, 11);
				if (bytesReceived > 0)
				{
					do
					{
						if (x - numx < 4096)
						{
							recv(sock, buf, x - numx, 0);
							MyOutFile << string(buf, x - numx);
						}
						else
						{
							recv(sock, buf, 4096, 0);
							MyOutFile << string(buf, 4096);
						}
						numx += 4096;
						ZeroMemory(buf, 4096);
					} while (numx < x);

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
						closesocket(sock);
						continue;
					}
					st.pathfile = "";
					numx = 0;
					ZeroMemory(st.numtext1, 11);
				}
			}
			// Close the file
			MyOutFile.close();
		}
		else if (strcmp(string(buf, 0, bytesReceived).c_str(), "recv") == 0)
		{
			ZeroMemory(buf, 4096);
			
			do
			{
				cout << "Nhap path file send: ";
				getline(cin, st.pathfile);
				ifstream file(st.pathfile, ios::in | ios::binary | ios::ate);

				size = file.tellg();
				if (size < 1515000000)
				{
					memblock = new char[size];
					file.seekg(0, ios::beg);
					file.read(memblock, size);
					file.close();
					break;
				}
				else { cout << "Yeu cau be hon 1.5gb" << endl;file.close(); }
			} while (1);
			for (int i = st.pathfile.length() - 1;i >= 0;i--)
			{
				if (st.pathfile.at(i) == '\\')
				{
					st.filename = st.pathfile.substr(i + 1);
					break;
				}
			}

			//wait for client send data
			cout << string("Ready", 0, 5) << endl;
			//echo message back to client
			bytesReceived = recv(sock, buf, 4096, 0);
			// tin hieu da ket noi
			send(sock, st.filename.c_str(), st.filename.length(), 0);

			if (bytesReceived == SOCKET_ERROR)
			{
				cerr << "GUi that bai" << endl;

			}
			if (bytesReceived == 0)
			{
				cout << "Client disconnect" << endl;
			}
			//

			sprintf_s(st.numtext1, "%d", (int)size);

			send(sock, st.numtext1, 11, 0);
			while (st.numtext < (int)size)
			{
				send(sock, memblock + st.numtext, 4096, 0);
				st.numtext += 4096;
			}
		

			if (st.numtext >= (int)size)
				cout << "Gui thanh cong" << endl;
			else
			{
				cout << "Gui that bai" << endl;
				closesocket(sock);
				continue;
			}
			ZeroMemory(buf, 4096);
			st.numtext = 0;
			delete[] memblock;
		}

	closesocket(sock);
	} while (true);
	/*closesocket(sock);*/
	WSACleanup();
}

string gethostip()
{
	string line;
	string hostip;
	ifstream IPFile;
	int offset;
	char* search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern

	system("ipconfig > ip.txt");

	IPFile.open("ip.txt");
	if (IPFile.is_open())
	{
		while (!IPFile.eof())
		{
			getline(IPFile, line);
			if ((offset = line.find(search0, 0)) != string::npos)
			{
				//   IPv4 Address. . . . . . . . . . . : 1
				//1234567890123456789012345678901234567890     
				line.erase(0, 39);
				hostip = line;
				IPFile.close();
			}
		}
	}
	return hostip;
}