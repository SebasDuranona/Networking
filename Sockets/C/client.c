#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5001
#define BUFFER_SIZE 1024

int is_numeric(const char* str) {
    while(*str) {
        if(!isdigit(*str) && *str != '-' && *str != '+') // Allow for negative numbers
            return 0;
        str++;
    }
    return 1;
}

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];
    char client_name[] = "Sebastian's Client";
    int client_number;

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    
    printf("Initialized.\n");
    
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    
    printf("Socket created.\n");
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    while(1) {
        printf("Enter a number 1-100: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0; // Remove newline if present

        if (!is_numeric(input)) {
            printf("Error: Non-numeric input. Please enter a valid number.\n");
            continue;
        }

        client_number = atoi(input);
        break;
    }

    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("Connect error");
        return 1;
    }
    
    printf("Connected to server.\n");
    
    sprintf(buffer, "%s|%d", client_name, client_number);
    send(client_socket, buffer, strlen(buffer), 0);
    
    printf("Message sent to server.\n");
    
    if(recv(client_socket, buffer, BUFFER_SIZE, 0) < 0) {
        puts("recv failed");
        return 1;
    }
    
    if(client_number < 1 || client_number > 100) {
        printf("Out-of-range number sent. Server should terminate.\n");
    } else {
        char server_name[BUFFER_SIZE];
        int server_number;
        sscanf(buffer, "%[^|]|%d", server_name, &server_number);
        
        printf("Client name: %s\n", client_name);
        printf("Server name: %s\n", server_name);
        printf("Client's number: %d\n", client_number);
        printf("Server's number: %d\n", server_number);
        
        int sum = client_number + server_number;
        printf("Sum: %d\n", sum);
    }
    
    closesocket(client_socket);
    WSACleanup();
    
    return 0;
}