#ifndef SOCKET_H

#define SOCKET_H

#include <winsock2.h>
#include <string>

enum TipoSocket
{
    SocketBloqueado,
    SocketDesbloqueado
};

class Socket
{
    public:

    virtual ~ Socket();
    Socket(const Socket&);
    Socket & operator = (Socket&);
    std :: string RecibirLinea();
    std :: string RecibirBytes();

    void Cerrar();

    // Este parámetro no es una referencia constante, porque este modifica el std anterior.
    void EnviarLinea(std :: string);

    // Este parámetro es todo lo CONTRARIO al anterior, porque este no modifica el std posterior.
    void EnviarBytes(const std :: string&);

    protected:

    friend class ServidorSocket;
    friend class SocketSelected;

    Socket(SOCKET s);
    Socket();

    SOCKET s_;

    int * refCounter_;

    private:

    static void Comenzar();
    static void Finalizar();
    static int noSockets_;
};

class ClienteSocket : public Socket
{
    public: ClienteSocket(const std :: string & host, int puerto);
};

class ServidorSocket : public Socket
{
    public: ServidorSocket(int puerto, int conexiones, TipoSocket type = SocketBloqueado);

    Socket * Aceptar();
};

class SocketSelected
{
    public : SocketSelected(Socket const * const s1, Socket const * const s2 = NULL, TipoSocket type = SocketBloqueado);

    bool Legible(Socket const * const s);

    private : fd_set fds_;
};

#endif