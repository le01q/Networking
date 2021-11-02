#include <stdio.h>
#include <winsock.h>

// Winsock library
#pragma comment(lib, "ws2_32.lib")

#define RECEIVE_BUFFER_SIZE 32

void error(char *message)
{
    fprintf(stderr, "%s: %d", message, WSAGetLastError());
    exit(EXIT_FAILURE);
    return;
}

void close(SOCKET so)
{
    closesocket(so);
    WSACleanup();
    return;
}

int main(int argc, char *argv[])
{
    SOCKET so;
    WSADATA wsa;
    char *server_ip, message[200], buffer[RECEIVE_BUFFER_SIZE];
    unsigned short server_port;
    int bytes_received;
    struct sockaddr_in server;

    if (argc != 3)
    {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        error("Failed WSAStartup");

    if ((so = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        error("Error on created the socket");

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(server_port);

    if (connect(so, (struct sockaddr *)&server, sizeof server) < 0)
        error("Failed to connect");

    printf("Connected to %s:%d\n", server_ip, server_port);

    while (strcmp(message, "QUIT") != 0)
    {
        if ((bytes_received = recv(so, buffer, RECEIVE_BUFFER_SIZE, 0)) == SOCKET_ERROR)
            error("Failed on receive");

        buffer[bytes_received] = '\0';
        printf("[Server]: %s", buffer);

        fgets(message, RECEIVE_BUFFER_SIZE, stdin);
        fflush(stdin);
        
        printf("[You]: %s", message);
        send(so, message, strlen(message) + 1, 0);
    }

    closesocket(so);
    WSACleanup();
    return 0;
}
