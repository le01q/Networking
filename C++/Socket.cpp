#include <Socket.h>
#include <iostream>

using namespace std;

int Socket :: noSockets_ = 0;

void Socket :: Comenzar()
{
    if (!noSockets_)
    {
        WSADATA info;

        if (WSAStartup (MAKEWORD(2,0), &info))
        {
            throw "Error al iniciar WSA";
        }
    }
    ++noSockets_;
}

void Socket :: Finalizar()
{
    WSACleanup();
}

Socket :: Socket(SOCKET s) : s_ (s)
{
    Comenzar();
    refCounter_ = new int(1);
}

Socket :: ~ Socket()
{
    if (! -- (*refCounter_))
    {
        Cerrar();
        delete refCounter_;
    }
    --noSockets_;
    if (!noSockets_) Finalizar();
}

Socket :: Socket(const Socket& o)
{
    refCounter_ = o.refCounter_;
    (*refCounter_ )++;
    s_ = o.s_;
    noSockets_++;
}

Socket& Socket :: operator = (Socket& o)
{
    (*o.refCounter_)++;
    refCounter_= o.refCounter_;
    s_ = o.s_;
    noSockets_++;
    return *this;
}

void Socket :: Cerrar()
{
    closesocket(s_);
}

std :: string Socket :: RecibirLinea()
{
    std :: string ret;

    while (1)
    {
        char r;

        switch (recv(s_, &r, 1, 0))
        {
            case 0: return ret;
            case -1: return "";

        }
        ret += r;
        if (r == '\n') return ret;
    }
}

void Socket :: EnviarLinea(std :: string s)
{
    s += '\n';
    send(s_, s.c_str(), s.length(), 0);
}

void Socket :: EnviarBytes(const std :: string& s)
{
    send(s_, s.c_str(), s.length(), 0);
}

ServidorSocket :: ServidorSocket(int puerto, int conexiones, TipoSocket type)
{
    sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));

    sa.sin_family = PF_INET;
    sa.sin_port = htons(puerto);

    s_ = socket(AF_INET, SOCK_STREAM, 0);

    if (s_ == INVALID_SOCKET)
    {
        throw "Socket invalido";
    }

    if (type == SocketDesbloqueado)
    {
        u_long arg = 1;
        ioctlsocket(s_, FIONBIO, &arg);
    }

    // Enlazar el socket a la dirección de Internet.
    if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        closesocket(s_);
        throw "Socket invalido";
    }

    listen(s_, conexiones);
}

Socket* ServidorSocket :: Aceptar()
{
    
}

ClienteSocket :: ClienteSocket(const std :: string& host, int puerto) : Socket()
{
    //...
}

SocketSelected :: SocketSelected(Socket const * const s1, Socket const * const s2, TipoSocket type)
{
    //...
}

bool SocketSelected :: Legible(Socket const* const s)
{
    //...
}

