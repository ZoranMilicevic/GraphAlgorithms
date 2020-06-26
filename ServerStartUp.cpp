#include <iostream>
#include <atomic>
#include <thread>
#include "ServerStartUp.h"

using namespace std;

//global vars
atomic<bool> listen_for_connections = true;


SOCKET create_listening_socket()
{
	//initialize a winsoc
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock, Err #" << wsOk << endl;
		WSACleanup();
		return 0;
	}

	//create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cout << "Can't create socket, Err #" << WSAGetLastError() << endl;
		closesocket(listening);
		WSACleanup();
		return 0;
	}

	//bind the an ip address and port to a socket
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(54000);	//network is big endian
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listening, (sockaddr*)&addr, sizeof(addr));

	//tell winsock that the socket is for listening
	listen(listening, SOMAXCONN);

	return listening;
}

void listen_on_listening_socket(SOCKET listening)
{
	while (listen_for_connections) {
		//wait for a connection
		sockaddr_in client;
		int size_of_client = sizeof(client);
		SOCKET client_socket = accept(listening, (sockaddr*)&client, &size_of_client);
		if (client_socket == INVALID_SOCKET)
		{
			cout << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			closesocket(client_socket);
			WSACleanup();
			return;
		}

		char host[NI_MAXHOST];
		char service[NI_MAXSERV];

		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);


		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			cout << host << " connected on port " << service << endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << " connected on port " << ntohs(client.sin_port);
		}

		//spawn a thread for client
		thread thr(listen_on_client_socket, client_socket);
		thr.detach();
	}

	//close client socket
	closesocket(listening);
}

void listen_on_client_socket(SOCKET client_socket)
{
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);
		//wait for data
		int bytes_received = recv(client_socket, buf, 4096, 0);
		if (bytes_received == SOCKET_ERROR)
		{
			//todo print error
		}

		buf[bytes_received + 1] = '\0';
		string received_string(buf);

		if (received_string == "disconnect")
		{
			break;
		}

		//echo message back to client
		send(client_socket, buf, bytes_received + 1, 0);
	}

	//close client socket
	closesocket(client_socket);

	//shutdown winsock
	WSACleanup();
}


int server_init() 
{
	SOCKET listening = create_listening_socket();
	listen_on_listening_socket(listening);
	return 0;
}

int main()
{
	server_init();
}