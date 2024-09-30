#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5001
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[BUFFER_SIZE];
    char server_name[] = "Sebastian's Server";
    int server_number = 50;  // Server's chosen number

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    
    printf("Initialized.\n");
    
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    
    printf("Socket created.\n");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    if(bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("Bind done.\n");
    
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed with error code : %d", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("Waiting for incoming connections...\n");
    
    c = sizeof(struct sockaddr_in);
    
    while(1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed with error code : %d", WSAGetLastError());
            continue;
        }
        
        printf("Connection accepted.\n");
        
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
            printf("recv failed with error code : %d", WSAGetLastError());
            closesocket(client_socket);
            continue;
        }
        
        char client_name[BUFFER_SIZE];
        int client_number;
        sscanf(buffer, "%[^|]|%d", client_name, &client_number);
        
        printf("Client name: %s\n", client_name);
        printf("Server name: %s\n", server_name);
        printf("Client's number: %d\n", client_number);
        printf("Server's number: %d\n", server_number);
        
        if(client_number < 1 || client_number > 100) {
            printf("Client number out of range. Terminating server.\n");
            closesocket(client_socket);
            break;
        }
        
        int sum = client_number + server_number;
        printf("Sum: %d\n", sum);
        
        sprintf(buffer, "%s|%d", server_name, server_number);
        if (send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            printf("send failed with error code : %d", WSAGetLastError());
        } else {
            printf("Response sent to client.\n");
        }
        
        closesocket(client_socket);
        printf("Client connection closed. Waiting for new connections...\n");
    }
    
    closesocket(server_socket);
    WSACleanup();
    
    return 0;
}