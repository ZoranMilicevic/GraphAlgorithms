#pragma once
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main();
int server_init();
SOCKET create_listening_socket();
void listen_on_listening_socket(SOCKET listening);
void listen_on_client_socket(SOCKET client_socket);
