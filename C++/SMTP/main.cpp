#include "SMTP.h"

int main(int argc, char *argv[])
{
    SOCKET sockfd = NULL;

    ConnectToHost(&sockfd, 25, "127.0.0.1");
    SendAnything(sockfd, "host.local");

    MailFrom(sockfd);
    MailTo(sockfd);

    MailCompose(sockfd);
    MailCloseConnection(sockfd);
    CloseHostConnection(sockfd);

    system("pause");

    return EXIT_SUCCESS;
}