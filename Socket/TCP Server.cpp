#include <Windows.h>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")


#define buff_len 2000		// Receive Data Buffer Length
int main() {

	WSADATA wsa;
	SOCKET sock;

	// Init Windows Socket Api
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		printf("WSAInit Failed. Code: %d", WSAGetLastError());
		return 1;
	}
	printf("WSA Initialized.\n");


	// Create Socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   // AF_INET: Address Family IPv 4
	if(sock == INVALID_SOCKET) {
		printf("Could not create Socket: %d", WSAGetLastError());
		return 1;
	}
	printf("Socket Created.\n");


	// Bind
	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	ServerAddr.sin_port = htons(8888);

	int res = bind(sock, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if(res == SOCKET_ERROR) {
		// Bind Failed
		printf("Bind Failed. ERROR: %d", WSAGetLastError());
		return 1;
	}
	printf("Server Started.\n");


	// Listen
	listen(sock, SOMAXCONN);   // places socket in a state for an incoming connection
	printf("Waiting for incomming connection\n");


	// Accept Connection
	SOCKET client_sock;
	SOCKADDR_IN client;
	int size = sizeof(SOCKADDR_IN);
	client_sock = accept(sock, (SOCKADDR*)&client, &size);
	if(client_sock == INVALID_SOCKET) {
		printf("Accept Faild.\n");
		return 1;
	}
	printf("Connection Accepted.\n");


	// Print Client Information 
	size = sizeof(client);
	printf("Clinet IP: %s\n", inet_ntoa(client.sin_addr));
	printf("Clinet Port: %d\n", htons(client.sin_port));


	// Receive Data
	printf("Waiting to receive data...\n");
	char receive_buff[1024];
	res = recv(client_sock, receive_buff, 1024, 0);
	if(res == SOCKET_ERROR) {
		printf("Receive Failed. ERROR Code: %d\n", WSAGetLastError());
		return 1;
	}
	printf("Data Received from Client.\n\n");
	if(res == 1024) res--;
	receive_buff[res] = '\0';
	printf("%s\n\n", receive_buff);


	// Send Data to Client
	res = send(client_sock, receive_buff, res, 0);
	if(res == SOCKET_ERROR) {
		printf("Send Data Failed\n");
		return 1;
	}
	printf("Data Sent\n");


	// Close Client Socket
	res = closesocket(client_sock);
	if(res != 0) {
		printf("Can not close client socket\n");
	}
	printf("Client Socket Closed\n");


	// Close Server Socket
	res = closesocket(sock);
	if(res != 0) {
		printf("Can not Close Socket\n");
	}
	printf("Server Socket Closed\n");


	// Clean WSA after working with sockets
	res = WSACleanup();    // Unload winsock library
	if(res != 0) {
		printf("WSA Cleanup Failed\n");
	}
	printf("WSA Cleanup finished\n");
	

	getchar();
	return 0;
}










