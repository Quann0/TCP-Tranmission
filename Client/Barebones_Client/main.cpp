#include <iostream>
#include <string>
#include <typeinfo>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	//// Create socket
	//SOCKET sock = socket(AF_INET, SOCK_STREAM, 6);
	//if (sock == INVALID_SOCKET)
	//{
	//	cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
	//	WSACleanup();
	//	return;
	//}

	//// Fill in a hint structure
	//sockaddr_in hint;
	//hint.sin_family = AF_INET;
	//hint.sin_port = htons(port);
	//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//// Connect to server
	//int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	//if (connResult == SOCKET_ERROR)
	//{
	//	cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
	//	closesocket(sock);
	//	WSACleanup();
	//	return;
	//}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;
	int connResult;
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
		/*if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			closesocket(sock);
			WSACleanup();
			return;
		}*/
		cout << "(send/recv)> ";
		getline(cin, userInput);
	if (userInput == "recv") 
	{
		
			// Prompt the user for some text
			if (userInput.size() > 0)		// Make sure the user has typed in something
			{
				// Send the text
				int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
				if (sendResult != SOCKET_ERROR)
				{
					// Wait for response
					ZeroMemory(buf, 4096);
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						// Echo response to console
						cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
					}
				}
			}
	}
	else
	{
			// Prompt the user for some text

			if (userInput.size() > 0)		// Make sure the user has typed in something
			{
				// Send the text
				int sendResult = send(sock, "quan", 4 + 1, 0);
				if (sendResult != SOCKET_ERROR)
				{
					// Wait for response
					ZeroMemory(buf, 4096);
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						cout << string(buf, 0, bytesReceived) << endl;
						// Echo response to console
						cout << "SERVER> " << string("Gui thanh cong", 0, 14) << endl;
					}
				}
			}
	}
	recv(sock, buf, 4096, 0);
	connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	/*if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;

	}*/
	closesocket(sock);
	//} while (userInput.size() > 0);
	} while (true);
	// Gracefully close down everything
	/*closesocket(sock);*/
	WSACleanup();
}
