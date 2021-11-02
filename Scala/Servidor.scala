import java.io.{BufferedReader, InputStreamReader, PrintWriter}
import java.net.{ServerSocket, Socket}

object Servidor extends App
{
  var serverSocket = ServerSocket = new ServerSocket(1027)

  while(true)
  {
    val socket: Socket = serverSocket.accept()
    val entrada: BufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream))
    val salida: PrintWriter = new PrintWriter(socket.getOutputStream)

    val consulta = String = null;
    val continuar = Boolean = true;

    while(continuar)
    {
      consulta = entrada.readLine()

      if (consulta != null)
      {
        println(consulta + " desde el cliente...")
        salida.println(consulta + " desde el servidor...")
        salida.flush()
        continuar = false
      }
    }

    entrada.close()
    salida.close()
    socket.close()

  }
  serverSocket.close()
}
