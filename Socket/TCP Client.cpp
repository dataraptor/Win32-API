#include <Windows.h>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

/*Steps: -----------
  1. WSAStartup
  2. Create Socket
  3. Connect 
  4. Send Data
  5. Receive Data
  6. Close Socket
  7. WSACleanup
  */


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


	// Connect To Server
	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("74.125.130.139");
	ServerAddr.sin_port = htons(80);

	int ret = connect(sock, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr));
	if(ret != 0) {
		// Connection Failed
		printf("Connection to Server Failed\n");
		return 1;
	}
	printf("Connected to Server\n");


	// Send Request
	char *message = "GET / HTTP/1.1\r\n\r\n";
	int bytesent = send(sock, message, strlen(message), 0);
	if(bytesent == SOCKET_ERROR) {
		printf("Send Error\n");
		return 1;
	}
	printf("Message Sent. Lenth:%d\n\n", bytesent);


	// Print Client Information
	SOCKADDR_IN senderinfo;
	int nlen = sizeof(senderinfo);
	getsockname(sock, (SOCKADDR*)&senderinfo, &nlen);
	printf("Client: Sender IP Used: %s\n", inet_ntoa(senderinfo.sin_addr));
	printf("Client: Sender Port Used %d\n", htons(senderinfo.sin_port));
	printf("Client: Data Sent:  %s\n", message);


	// Receive Replay from server
	char server_replay[buff_len];
	int recev_len = recv(sock, server_replay, buff_len, 0);
	if(recev_len == SOCKET_ERROR) {
		printf("Receive Failed\n");
		return 1;
	}
	printf("Receive Successful\n");


	// Prepare to Print Received data
	if(recev_len == buff_len) recev_len--;
	server_replay[recev_len] = '\0';
	printf("\n\n%s", server_replay);


	// Close Socket
	int res = closesocket(sock);
	if(res != 0) {
		printf("Can not Close Socket");
	}


	// Clean WSA after working with sockets
	res = WSACleanup();    // Unload winsock library
	if(res != 0) {
		printf("WSA Cleanup Failed");
	}
	printf("\n\nWSA Cleanup finished");


	getchar();
	return 0;
}










