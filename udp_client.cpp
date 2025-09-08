#include <ws2tcpip.h>
#include <stdio.h>

//UDP Client Steps
//Step 1: Start up Winsock API
//Step 2: Create socket
//Step 3: Send and receive data

int main(){
	printf("Starting UDP client...\n");

    WSADATA wsa_data;
    int wsa_err;
    WORD w_version_requested = MAKEWORD(2,2);
    wsa_err = WSAStartup(w_version_requested, &wsa_data);
    if(wsa_err != 0){
		printf("The winsock dll not found!\n");
        return 0;
    } else {
		printf("The Winsock dll found! Status: %s\n", wsa_data.szSystemStatus);
    }

    SOCKET client_socket;
    client_socket = INVALID_SOCKET;
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(client_socket == INVALID_SOCKET){
        printf("Error at socket(): %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
	} else {
		printf("socket is OK!\n");
	}

    sockaddr_in client_service;
    client_service.sin_family = AF_INET;
	InetPton(AF_INET, "127.0.0.1", &client_service.sin_addr.s_addr); 
    client_service.sin_port = htons(55555);
	int client_service_size = sizeof(client_service);
    
	while (1) {
		char send_buffer[200];
		printf("Enter the message: ");
		if (fgets(send_buffer, 200, stdin) != NULL) {
			size_t len = strcspn(send_buffer, "\n");
			if (send_buffer[len] == '\n') {
				send_buffer[len] = '\0';
			} else {
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {};
			}
		}		
		int sent_bytes = sendto(client_socket, send_buffer, strlen(send_buffer), 0, (SOCKADDR *)&client_service, client_service_size);
		if(sent_bytes == SOCKET_ERROR){
			printf("sendto() failed: %d\n", WSAGetLastError());
		} else {
			printf("sendto() succeeded: %d\n", sent_bytes);
		}

		char recv_buffer[200];
		int recv_bytes = recvfrom(client_socket, recv_buffer, sizeof(recv_buffer) - 1, 0, (SOCKADDR *)&client_service, &client_service_size);		
		if (recv_bytes == SOCKET_ERROR) {
			printf("recvfrom() failed: %d\n", WSAGetLastError());
		} else {
			recv_buffer[recv_bytes] = '\0';
			printf("recvfrom() succeeded: %s\n", recv_buffer);
		}
	}    
}