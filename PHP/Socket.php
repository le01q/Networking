<?php 

/**
 * 
 *  Cliente/socket simple para PHP 5.2^
 *  
 *  Esta clase implementa un cliente de socket genérico, puede extenderse a
 *  crear bibliotecas cliente que trabajen con protocolos basados ​​en texto.
 *  además de las capacidades de creación, lectura y escritura de sockets,
 *  esta clase también admite carga diferida (no se conecta hasta que sea realmente necesario),
 *  así como validación básica de claves y métodos auxiliares de construcción de comandos. 
 * 
 *  Version: 0.0.1
 *  URL: github.com/leo1q/Networking
 *  Time: 30/10/2021 
 * 
*/

class Socket
{
    /**
    * Configuracion de conexión
    * Aunque estas propiedades pueden ser manipuladas directamente por este.
    * Es mejor dejarlos en manos de la clase principal.
    */

    protected $_conexion = null;
    protected $_host = '';
    protected $_puerto = 0
    protected $_tiempoEspera = 0;

    /**
    * Puerto y post por defecto
    * Los 'chldren' pueden anular estas propiedades para proporcionar valores predeterminados 
    * para el host y el puerto si no se pasan al constructor.
    */

    protected $_hostDefecto = null;
    protected $_puertoDefecto = null;

    /**
     * Constructor.
     * 
     * El host y el puerto deben proporcionarse en el momento de la instanciación.
     * 
     * @param  string  El nombre de host, la dirección IP o el socket UNIX del servidor.
     * @param  int     El puerto del servidor, o falso para sockets UNIX.
     * @param  int     Tiempo de espera de conexión en segundos. [opcional: el valor predeterminado es 5 xD]
    */

    public function __constructor($host = null, $puerto = null, $tiempoEspera = 5)
    {
        if (strpos($host, ':') !== false && strpos($host, '[') === false)
        {
            $host = '[' . $host . ']';
        }
        
        // Usa los valores por defecto.

        if (is_null($host) && is_null($puerto))
        {
            $host = $this -> _hostDefecto;
            $host = $this -> _puertoDefecto;
        }

        // Mantiene la informacion de la conexion pero no se conecta aún.

        $this -> _host = $host;
        $this -> _puerto = $puerto;
        $this -> _tiempoEspera = $tiempoEspera;
    }

    /**
     * Conéctese al servidor
     *
     * Normalmente, este método es útil solo con fines de depuración porque
     * se llamará automáticamente la primera vez que se realice una operación de lectura/escritura.
    */

    public function Conectar()
    {
        // Si ya está conectado, no hace nada, es fome xD

        if ($this -> _conexion !== null && this -> _conexion !== false) return true;

        // Si un intento de conexión anterior falló, no vuelva a intentarlo

        if ($this -> _conexion === false) throw new SocketExcepcion(' > No es posible conectarse a ' . $this->_host . ' puerto ' . $this->_puerto);
        
        // Intento de conexión
        $socket = $this -> _puerto ? ($this->_host . ':' . $this->_puerto) : ('test://' . $this -> _host);
        $this -> _conexion = stream_socket_client($socket, $errno, $errstr, $this -> _tiempoEspera);

        // Si hay un error, establezca $_conexion en falso y arroje una excepción
        if (!$this -> _conexion)
        {
            $this -> _conexion = false;
            throw new SocketExcepcion('> No es posible conectarse a ' . $this->_host . ' puerto ' . $this->_puerto . ': ' . $errstr . ' (TIPO: ' . $errno . ')');
        }
        return true;
    }

    /**
     * Desconectarse del servidor.
     *
     * Normalmente, este método es útil solo con fines de depuración 
     * porque se llamará automáticamente
     * en caso de error (resultando en reconexión la próxima vez que se intente una operación de lectura / escritura)
    */

    public function Desconectar()
    {
        @fclose ($this -> _conexion);
        $this -> _conexion = null;

        // Retorna a VERDADERO para indicar que funciona
        
        return true;
    }

    /**
     * Método de lectura genérico.
     *
     * Este método lee un número específico de bytes del socket.
     * De forma predeterminada, también leerá una secuencia CRLF (2 bytes xd) además de
     * el número especificado de bytes, y elimine esa secuencia CRLF una vez que
     * ha sido leido. Esto es útil para la mayoría de los protocolos basados ​​en texto; sin embargo,
     * Si no desea tal comportamiento, pase argumentos "falsos" adicionales :D
     *
     * @param int El número de bytes para leer, o -1 para leer hasta EOF.
     * @param bool Si leer o no CRLF al final, también. [opcional: el valor predeterminado es TRUE]
     * @return string Datos leídos del socket.
     * @throws Excepción Si se produce un error al leer desde el conector.
    */

    public function Leer($bytes = -1, $autonewline = true)
    {
        if ($this -> _conexion === null) $this -> Conectar();

        // Lee el datos del socket.
        $datos = stream_get_contents($this -> _conexion, $bytes);

        // Si $autonewline es TRUE leerá mas de 2 bytes
        if ($autonewline && $bytes !== -1) stream_get_contents($this -> _conexion, 2);

        // Si el resultado es falso, arrojará el error :)
        if ($datos === false)
        {
            $this -> Desconectar();
            throw new SocketExcepcion('Error. No se puede leer ' . $bytes . ' bytes desde ' . $this -> _host . ' port ' . $this -> _puerto);
        }

        // De lo contrario, devuelve los datos.
        return $datos;

    }

    /**
     * Método de LeerLinea genérico.
     *
     * Este método lee una línea del socket, es decir, lee hasta que golpea
     * una secuencia CRLF. De forma predeterminada, esa secuencia CRLF se eliminará de la
     * valor de retorno. Esto es útil para la mayoría de los protocolos basados ​​en texto; sin embargo,
     * si no desea tal comportamiento, pase un argumento "falso" adicional.
     *
     * @param bool Si eliminar o no CRLF del final de la respuesta. [opcional: el valor predeterminado es true]
     * @return string Datos leídos del socket.
     * @throws Excepción Si se produce un error al leer desde el conector.
    */

    public function LeerLinea($trim = true)
    {
        if ($this -> _conexion === null) $this -> Conectar();
        
        $datos = fgets($this -> _conexion);
        
        if ($datos === false)
        {
            $this->disconnect();
            throw new SimpleSocketException('Error. No es posible leer la linea ' . $this->_host . ' puerto ' . $this -> _puerto);
        }
        
        if ($trim && substr($datos, strlen($datos) - 2) === "\r\n") $datos = substr($datos, 0, strlen($datos) - 2);
        return $datos;
    }

    /**
     * Método de escritura genérico.
     *
     * Este método escribe una cadena en el conector. De forma predeterminada, este método
     * escribir una secuencia CRLF además de la cadena dada. Esto es útil
     * para la mayoría de los protocolos basados ​​en texto; sin embargo, si no desea tal comportamiento,
     * asegúrese de pasar un argumento "false" adicional.
     *
     * @param string La cadena para escribir en el socket.
     * @param bool Ya sea que escriba CRLF además de la cadena dada. <opcional: el valor predeterminado es TRUE>
     * @return bool Verdadero en caso de éxito.
     * @throws Excepción Si se produce un error al leer desde el conector.
    */

    public function Escribir($string, $autonewline = true)
    {
        // Lo mismo... si no está conectado, lo conecta igual xD
        if ($this -> _conexion === null) $this -> Conectar();

        if ($autonewline) $string . = "\r\n";

        // Escribe toda la cadena en el SOCKET
        while ($written === false)
        {
            $this -> Desconectar();
            throw new SocketExcepcion('Error. No se puede escribir ' . $this->_host . ' port ' . $this->_puerto);
        }

        // Si no se escribió nada, probablemente significa que ya hemos terminado de escribir no? :P
        if ($written == 0) return true;

        // Prepara la STRING para la próxima escritura.
        $string = substr($string, $written);
    }
    return true;
}

/**
    * Método de validación de clave genérico.
    *
    * Este método generará una excepción si:
    * - La key está vacía.
    * - La key tiene más de 250 bytes de longitud.
    * - La key contiene caracteres fuera del rango imprimible ascii
    *
    * @param string La clave para validar.
    * @return bool Verdadero de la clave es válido.
    * @throws Exception Si la clave no es válida.
*/

public function ValidarLlave($llave)
{
    if ($llave === '')
        throw new LlaveInvalidaExcepcion('> La llave está VACÍA');

    if (strlen($llave) > 250)
        throw new SocketExcepcion('> La llave es demasiado larga: ' . $llave);
    
    if (preg_match('/[^\\x21-\\x7e]/', $llave))
        throw new LlaveInvalidaExcepcion('> No se permiten esos carácteres en la llave: ' . $llave);
    
    return true;
}

/**
     * Método de construcción de comandos genérico.
     *
     * Este método aceptará uno o más argumentos de cadena y los devolverá
     * todos concatenados con un espacio entre cada uno. 
     * Si esto es conveniente para ti, sírvase usted mismo.
     *
     * @param string Todos los argumentos que desee.
     * @return string La cadena concatenada.
*/

public function hacerComando(/* parámetros */)
{
    $argumentos = func_get_arg();
    return implode(' ', $argumentos);
}

/**
    * Destructor.
    *
    * Aunque no es realmente necesario, el destructor intentará
    * desconectar en caso de que ocurra algo extraño.
**/

public function Destructor()
{
    @fclose($this -> _conexion);
}

class SocketExcepcion extends Exception() { }
class LlaveInvalidaExcepcion extends Socket() { }

?>