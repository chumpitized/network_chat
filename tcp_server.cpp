#include <ws2tcpip.h>
#include <stdio.h>

//TCP Server Steps
//Step 1: Start up Winsock API
//Step 2: Create socket
//Step 3: bind() socket
//Step 4: listen()
//Step 5: accept() connection
//Step 6: Receive and send data

int main(){
	printf("Starting TCP server...\n");

    WSADATA wsa_data;
    int wsa_err;
    WORD w_version_requested = MAKEWORD(2,2);
    wsa_err = WSAStartup(w_version_requested, &wsa_data);

    if (wsa_err != 0) { 
        printf("The Winsock dll not found!\n");
        return -1;
    } else {
        printf("The Winsock dll found!\nThe status: %s\n", wsa_data.szSystemStatus);
    }

    SOCKET server_socket = INVALID_SOCKET;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
		printf("Error at socket(): %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    } else {
		printf("socket() is OK!\n");
    }

	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr); 
	service.sin_port = htons(55555);

	if (bind(server_socket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	} else {
		printf("bind() is OK!\n");
	}

    //if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
	if (listen(server_socket, 1) == SOCKET_ERROR) {
        printf("listen(): Error listening on socket...  %d\n", WSAGetLastError());
		WSACleanup();
		closesocket(server_socket);
		return -1;
    } else {
		printf("listen() is OK, waiting for new connections...\n");
    }

	SOCKET accept_socket = INVALID_SOCKET;

	while (1) {
		if (accept_socket == INVALID_SOCKET) {
			accept_socket = accept(server_socket, NULL, NULL);
			if (accept_socket == INVALID_SOCKET) {
				printf("accept failed: %d\n", WSAGetLastError());
				continue;
			}
			printf("Accepted connection!\n");
		}

		char recv_buffer[200];
		int recv_bytes = recv(accept_socket, recv_buffer, sizeof(recv_buffer) - 1, 0);
		if (recv_bytes < 0) {
			printf("Server recv error: %d\n", WSAGetLastError());
			closesocket(accept_socket);
			accept_socket = INVALID_SOCKET;
			continue;
		} else if (recv_bytes == 0) {
			printf("Connection closed by client.\n");
			closesocket(accept_socket);
			accept_socket = INVALID_SOCKET;
			continue;
		}
		recv_buffer[recv_bytes] = '\0';
		printf("recvfrom succeeded: %s\n", recv_buffer);

		char send_buffer[200];
		printf("Enter message: ");
		if (fgets(send_buffer, 200, stdin) != NULL) {
			size_t len = strcspn(send_buffer, "\n");
			if (send_buffer[len] == '\n') {
				send_buffer[len] = '\0';
			} else {
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {}
			}
		}
		int bytes_sent = send(accept_socket, send_buffer, strlen(send_buffer), 0);
		if (bytes_sent == SOCKET_ERROR) {
			printf("send() failed: %d\n", WSAGetLastError());
			closesocket(accept_socket);
			accept_socket = INVALID_SOCKET;
			continue;
		}

		printf("Server sent: %d bytes\n",  bytes_sent);
	}

	return 0;
}