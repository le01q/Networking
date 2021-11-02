import java.io.{BufferedReader, InputStreamReader, PrintWriter}
import java.net.Socket

object Cliente extends App {
  var socket: Socket = new Socket("127.0.0.1", 1027)
  var entrada: BufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream))
  var salida: PrintWriter = new PrintWriter(socket.getOutputStream)

  // Definimos el mensaje:
  var Mensaje: String = "Prueba"

  println("Enviado al servidor: " + Mensaje)

  salida.println(Mensaje)
  salida.flush()

  var Respuesta: String = entrada.readLine()
  
  println("Recibido desde el servidor: " + Respuesta)
}
