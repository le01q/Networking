using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Net.Sockets;

// Creación de la clase

public class SocketNetwork : MonoBehaviour 
{
    public String host = "localhost";
    public Int32 puerto = 50000;

    internal Boolean socketListo = false;
    internal String entradaBuffer = "";
    
    TcpClient socketTCP;
    NetworkStream NetStream;

    StreamWriter EscritorSocket;
    StreamReader LectorSocket;

    void Actualizar()
    {
        string DatosRecibidos = LeerSocket();
        string UsoDeTecla = Input.inputString;

        if (UsoDeTecla != "")
        {
            entradaBuffer += UsoDeTecla;

            if (UsoDeTecla == "\n")
            {
                // Lo deja limpio al buffer y se lo envía.
                Debug.Log("Enviando: " + entradaBuffer);
                EscritorSocket(entradaBuffer);
                entradaBuffer = "";
            }
        }

        if (DatosRecibidos != "")
        {
            // Hace algo con los datos que recibimos
            // Y lo mostramos
            Debug.Log(DatosRecibidos);
        }
    }

    void SocketEncendido()
    {
        SocketSetup();
    }

    void AplicacionCerrada()
    {
        SocketApagado();
    }

    public void SocketSetup()
    {
        try
        {
            if (!socketListo)
            {
                TCP_Socket = new TcpClient(host, puerto);

                NetStream = TCP_Socket.GetStream();
                EscritorSocket = new StreamWriter(NetStream);
                LectorSocket = new StreamReader(NetStream);

                socketListo = true;
            }

        } 
        catch (Exception e)
        {
            // Algo malo paso.. xD
            Debug.Log("Hubo un error en el socket: " + e);    
        }
    }

    // Próximamente
}