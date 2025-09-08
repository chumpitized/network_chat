#include <ws2tcpip.h>
#include <stdio.h>

//UDP Server Steps
//Step 1: Start up Winsock API
//Step 2: Create socket
//Step 3: bind() socket
//Step 4: Receive and send data

int main(){
	printf("Starting UDP server...\n");

    WSADATA wsa_data;
    int wsa_err;
    WORD w_version_requested = MAKEWORD(2,2);
    wsa_err = WSAStartup(w_version_requested, &wsa_data);
    if (wsa_err != 0) { 
        printf("Winsock dll not found!\n");
        return -1;
    } else {
        printf("Winsock dll found! Status: %s\n", wsa_data.szSystemStatus);
    }

    SOCKET server_socket = INVALID_SOCKET;
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

	while (1) {
		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);

		char recv_buffer[200];
		int recv_bytes = recvfrom(server_socket, recv_buffer, sizeof(recv_buffer) - 1, 0, (SOCKADDR *)&client_addr, &client_addr_size);
		if (recv_bytes == SOCKET_ERROR) {
			printf("recvfrom() failed: %d\n", WSAGetLastError());
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
		int bytes_sent = sendto(server_socket, send_buffer, strlen(send_buffer), 0, (SOCKADDR *)&client_addr, client_addr_size);
		if (bytes_sent == SOCKET_ERROR) {
			printf("sendto() failed: %d\n", WSAGetLastError());
			continue;
		}

		printf("sentto() succeeded: %d bytes\n",  bytes_sent);
	}

	return 0;
}