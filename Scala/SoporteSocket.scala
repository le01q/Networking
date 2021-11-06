package HandlerSocket 
{
    import java.net._, java.util._, java.util.concurrent._, java.util.concurrent.atomic._, java.io._
    import org.slf4j._
    import scala.util.control._, scala.collection.mutable.ArrayBuffer

    // Configuracion inicial
    case class HandlerSocketConfiguracion (
        host: String,
        puerto1: Int = 9991,
        puerto2: Int = 9992,
        codificacionConexion: String = "utf-8",
        leerBuffer: Int = 1024 * 128,
        tiempoEspera: Int = 10000,
        tcpNoDelay: Boolean = true,
        mantenerPrendido: Boolean = true,
        reutilizarDireccion: Boolean = true,
        soTiempoEspera: Int = 0,
        lectorPs: Int = 12,
        escritorPs: Int = 1
    )

    sealed abstract class Operador() { def Simbolo() : String }

    case class Igualdad(Simbolo: String = "=") extends Operador
    case class MayorQue(Simbolo: String = ">") extends Operador
    case class MenorQue(Simbolo: String = "<") extends Operador
    case class MayorIgual(Simbolo: String = ">=") extends Operador
    case class MenorIgual(Simbolo: String = "<=") extends Operador
    case class Suma(Simbolo: String = "+") extends Operador

    // La hilera...

    case class Row(columnas: Array[String])
    {
        def columna(i : Int) : String = 
        {  
            if (i >= columnas.length) 
            {
                throw new java.lang.IllegalArgumentException(i + "No se puede porque no es valido para las columnas.")
            } else {
                columnas(i)   
            }
        }
    }

    case class Resultados(codigoError: Int, numeroColumnas: Int, columnas: Array[String])
    {
        // Proximamente...
    }

    case class EspecifIndiceAbierto(bd: String, tabla: String, columnas: Array[String], indice: String = "PRIMARY")
    {
        // Proximamente...
    }

    // Detalles importantes internos

    trait Sesiones
    {
        // Proximamente...
    }

    trait SoporteConexion
    {
        // Proximamente...
    }

    case class SoporteConexionSimple(conexion: Socket, codigoConexion: String, leerBufferTam: Int) extends SoporteConexion 
    {
        // Proximamente...
    }

    class ImplementacionSesion(id: Int, spec: EspecifIndiceAbierto, soporteCon: SoporteConexion) extends Sesiones
    {
        // Proximamente...
    }

    class SoloLectura(indiceid: Int, spec: EspecifIndiceAbierto, soporteCon: SoporteConexion) extends ImplementacionSesion(id, spec, soporteCon)
    {
        // Proximamente...
    } 

    trait SesionFabrica
    {
        // ...
    }

    class SesionFabricaImpl(config: HandlerSocketConfiguracion) extends SesionFabrica
    {
        // ...
    }

    object A
    {
        // Aqui proximamente...
    }

    class PlantillaCliente()
    {
        
    }

}