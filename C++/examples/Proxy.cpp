// This is a simple Proxy demonstration example with last Socket in C++ created!
// We will use 'Socket.h' to use some issues of this :P

#include <Socket.h>

// Default importations
#include <iostream>
#include <process.h>
#include <sstream>

int puertoProxy;
std :: string dirServidor;
int puertoServidor;

unsigned __stdcall RunProxyThread (void* a)
{
    Socket* s = (Socket*) a;
    ClienteSocket c(dirServidor, puertoServidor);

    while (1)
    {
        SocketSelected sel(s, &c);

        bool sigueConectado = true;

        if (sel.Legible(s))
        {
            std :: string Bytes = s -> RecibirBytes();

            c.EnviarBytes(Bytes);

            std :: cout << "[!] Servidor: " << Bytes << std :: endl;

            if (Bytes.empty()) sigueConectado = false;

        }

        if (! sigueConectado)
        {
            break;
        }
    }

    delete s;

    return 0;
}

// Principal

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Uso:" << std::endl;
        std::cout << "proxy <Puerto proxy> <Direccion server> <Puerto server>" << std::endl;
        std::cout << std::endl;
        std::cout << "  Este proxy tomará en <puerto proxy> y siempre que reciba" << std::endl;
        std::cout << "  una conexión, retransmite el tráfico al <Puerto del server> de su direccion" << std::endl;
        std::cout << "  Esto lo hace ideal para ver lo que un cliente SMTP intercambia con un servidor SMTP," << std::endl;
        std::cout << "  o igualmente lo que un cliente NNTP intercambia con un servidor NNTP." << std::endl << std::endl;
        return -1;
    }

    std :: stringstream s;

    s <<argv[1]; s >> puertoProxy; s.clear();

    dirServidor = argv[2];

    s << argv[3]; s >> puertoServidor;

    ServidorSocket in(puertoProxy, 5);

    while (1)
    {
        Socket* s = in.Aceptar();

        unsigned ret;

         _beginthreadex(0, 0, RunProxyThread,(void*) s,0,&ret);
    }

    return 0;
}