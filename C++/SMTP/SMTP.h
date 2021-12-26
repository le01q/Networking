#pragma once

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

unsigned int ConnectToHost(SOCKET *sockfd, unsigned int port, char *hostName);
void CloseHostConnection(SOCKET sockfd);

void SendAnything(SOCKET sockfd, char *str);
void MailCloseConnection(SOCKET sockfd);
void MailCompose(SOCKET sockfd);
void MailFrom(SOCKET sockfd);
void MailTo(SOCKET sockfd);
void ShowError(char *err);