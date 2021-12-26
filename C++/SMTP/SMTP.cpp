#include "SMTP.h"

void ShowError(char *err)
{
    printf("%s : %ld\n", err, WSAGetLastError());
}

unsigned int ConnectToHost(SOCKET *sockfd, unsigned int port, char *hostName)
{
    WSADATA wsadata = { 0 };

    if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
    {
        ShowError("ERROR: WSAStartup component.");
        return(1);
    }

    SOCKADDR_IN server = { 0 };
    HOSTENT *hostInformation = gethostbyname(hostName);
    char *ip = inet_ntoa(*(struct in_addr *) *hostInformation -> h_addr_list);

    server.sin_addr.S_un.S_addr = inet_addr(ip);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    *sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == *sockfd)
    {
        ShowError("ERROR: Socket invalid.");
        WSACleanup();
        return(1);
    }

    if (SOCKET_ERROR == connect(*sockfd, (SOCKADDR *) &server, sizeof(SOCKADDR_IN)))
    {
        ShowError("ERROR: Connection lost");
        WSACleanup();
        return(1);
    }

    char buffer[256] = { 0 };
    recv(*sockfd, buffer, 256, 0);
    printf("%s", buffer);

    return 0;
}

void CloseHostConnection(SOCKET sockfd)
{
    closesocket(sockfd);
    WSACleanup();
}

void SendAnything(SOCKET sockfd, char *str)
{
    char buffer[256] = "Hola ";
    strcat(strcat(buffer, str), "\r\n");

    if (SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    {
        ShowError("ERROR: Anything message problem detected :[");
    }

    memset(buffer, 0, 256);

    if (SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    {
        ShowError("ERROR: Problem to receive message/component");
    }

    printf("%s", buffer);
}

void MailCloseConnection(SOCKET sockfd)
{
    if (SOCKET_ERROR == send(sockfd, "QUIT\r\n", strlen("QUIT\r\n"), 0))
    {
        ShowError("ERROR: Data sent error.");
    }
}

void MailFrom(SOCKET sockfd)
{
    char userEmail[256] = { 0 };
    char buffer[256] = { 0 };

    printf("> Your email: ");
    gets(userEmail);

    strcat(buffer, "Mail from: <");
    strcat(buffer, userEmail);
    strcat(buffer, ">\r\n");

    if (SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    {
        ShowError("ERROR: Mail send content error.");
    }

    memset(buffer, 0, 256);

    if (SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    {
        ShowError("ERROR: Mail from receiving problem detected!");
    }

    printf("%s", buffer);
}

void MailTo(SOCKET sockfd)
{
    char userEmail[256], buffer[256] = { 0 };

    printf("> Destination: ");
    gets(userEmail);

    strcat(buffer, "Mail to: ");
    strcat(buffer, userEmail);
    strcat(buffer, ">\r\n");

    if (SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    {
        ShowError("ERROR: Mail to content problem detected.");
    }

    memset(buffer, 0, 256);

    if (SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    {
        ShowError("ERROR: Mail to receiving problem detected.");
    }

    printf("%s", buffer);
}

void MailCompose(SOCKET sockfd)
{
    char buffer[256] = { 0 };

    strcat(buffer, "DATA\r\n");

    if (SOCKET_ERROR == send(sockfd, buffer, strlen(buffer), 0))
    {
        ShowError("ERROR: Data sending problem detected.");
    }

    memset(buffer, 0, 256);

    if (SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    {
        ShowError("ERROR: Data receiving problem detected.");
    }

    printf("%s", buffer);
    memset(buffer, 0, 256);
    strcat(buffer, "\r\n");
    printf("Compose: ");
    gets(buffer);
    strcat(buffer, "\r\n.\r\n");

    char aux[256] = { 0 };
    strcat(strcat(aux, "\r\n"), buffer);

    if (SOCKET_ERROR == send(sockfd, aux, strlen(aux), 0))
    {
        ShowError("ERROR: Data composing error detected.");
    }

    memset(buffer, 0, 256);

    if (SOCKET_ERROR == recv(sockfd, buffer, 255, 0))
    {
        ShowError("ERROR: Data compose receiving error detected.");
    }

    printf("%s", buffer);
}