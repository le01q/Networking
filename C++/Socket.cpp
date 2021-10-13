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

std :: string Socket :: RecibirBytes()
{
    //...
}

std :: string Socket :: RecibirLinea()
{
    //...
}

void Socket :: EnviarLinea(std :: string s)
{
    //...
}

void Socket :: EnviarBytes(const std :: string& s)
{
    //...
}

ServidorSocket :: ServidorSocket(int puerto, int conexiones, TipoSocket type)
{
    //...
}

Socket* ServidorSocket :: Aceptar()
{
    //...
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

