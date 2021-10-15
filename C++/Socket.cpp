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
    SOCKET nuevo_socket = accept(s_, 0, 0);

    if (nuevo_socket == INVALID_SOCKET)
    {
        int RC = WSAGetLastError();

        if (RC == WSAEWOULDBLOCK)
        {
            return 0;
        } else {
            throw "Socket invalido";
        }
    }

    Socket* r = new Socket(nuevo_socket);

    return r;

}

ClienteSocket :: ClienteSocket(const std :: string& host, int puerto) : Socket()
{
    std :: string error;

    hostent *he;

    if ((he = gethostbyname(host.c_str())) == 0)
    {
        error = strerror(errno);
        throw error;
    }

    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(puerto);
    addr.sin_addr = * ((in_addr *) he -> h_addr);
    memset(& (addr.sin_zero), 0, 8);

    if (:: connect (s_, (sockaddr *)&addr, sizeof(sockaddr)))
    {
        error = strerror(WSAGetLastError());
        throw error;
    }
}

SocketSelected :: SocketSelected(Socket const * const s1, Socket const * const s2, TipoSocket type)
{
    FD_ZERO(&fds_);
    FD_SET(const_cast <Socket*> (s1) -> s_, &fds_);

    if (s2)
    {
        FD_SET(const_cast <Socket*> (s2) -> s_, &fds_);
    } 

    TIMEVAL tiempoValor;
    
    tiempoValor.tv_sec = 0;
    tiempoValor.tv_usec = 1;

    TIMEVAL *pTiempoValor;

    type == SocketDesbloqueado ? pTiempoValor = &tiempoValor : pTiempoValor = 0; 

    if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, pTiempoValor) == SOCKET_ERROR)
        throw "Error en seleccionar";
}

std :: string Socket :: RecibirBytes()
{
    std::string ret;
    char buf[1024];
 
    while (1) {
    u_long arg = 0;
    
    if (ioctlsocket(s_, FIONREAD, &arg) != 0)
      break;

    if (arg == 0)
      break;

    if (arg > 1024) arg = 1024;

    int rv = recv (s_, buf, arg, 0);
    if (rv <= 0) break;

    std::string t;

    t.assign (buf, rv);
    ret += t;
  }
 
  return ret;
}

bool SocketSelected :: Legible(Socket const* const s)
{
    if (FD_ISSET(s -> s_, &fds_)) return true;
        return false;
}