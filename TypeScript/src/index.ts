import { Socket } from 'net';

export interface Opciones {
    direccion: string
    puerto: number
    intentos: number
    tiempoEspera: number
}

// El resultado y su intento falla.
export interface Errores {
    intento: number
    error: Errores
}

export interface Resultados {
    opciones: Opciones
    latenciaMedia: number
    latenciaMinima: number
    latenciaMaxima: number
    errores: Errores[]
}

// El resultado del intento de conexión.
export interface IntentoConexion {
    numeroIntentos: number,
    
    resultado: {
        time?: number
        error?: Errores
    }
}

function conectar({ direccion, puerto, tiempoEspera }: Opciones): Promise<IntentoConexion['resultado']> {
    return new Promise(resolve => {
        // Creamos un socket TCP.
        const socket = new Socket()

        // Ahorre la hora actual para que podamos calcular la latencia.
        const startTime = process.hrtime()

        // Conectarse al host dado.
        socket.connect(puerto, direccion, () => {
            const [ segundos, nanosegundos ] = process.hrtime(startTime)
            
            // Convertir la latencia de nanosegundos a milisegundos para que sea más fácil trabajar con la salida.
            const tiempoParaConectar = (segundos * 1e9 + nanosegundos) / 1e6
            
            // Ya no necesitamos el socket, entonces deberíamos destruirlo xD
            socket.destroy()
            resolve({
                time: tiempoParaConectar
            })
        })

        // Nos aseguramos de detectar cualquier error arrojado por el socket :)
        socket.on('error', error => { 
            socket.destroy()
        })

        socket.setTimeout(tiempoEspera, () => {
            socket.destroy()
        })
    })
}

export async function Ping (opciones?: Partial<Opciones>, progreso?: (progreso: number, total: number) => void): Promise<Resultados> 
{
    const opts: Opciones = {
        direccion: '127.0.0.1',
        intentos: 10,
        puerto: 80,
        tiempoEspera: 3000,
        ...opciones
    }

    if (opts.puerto < 1)
        throw RangeError('Puerto negativo!')

    const resultadosConexion: IntentoConexion[] = []

    // Intento de conexión al host dado.
    for (let i = 0; i < opts.intentos; i++)
    {
        resultadosConexion.push({
            numeroIntentos: i + 1,
            resultado: await conectar(opts)
        })

        if (typeof progreso === 'function')
            progreso(i + 1, opts.tiempoEspera)
    }

    const result: Resultados = {
        latenciaMedia: NaN,
        errores: [],
        latenciaMaxima: 0,
        latenciaMinima: Infinity,
        opciones: opts
    }

    let sumaLatencia: number = 0

    for (const intento of resultadosConexion) {
        // Si el tiempo está indefinido, se asume que hay un error.
        if (typeof intento.resultado.time === 'undefined') {
            /*result.errores.push({
                intento: intento.numeroIntentos,
                error: intento.resultado.error || Error('Error desconocido!')
            })
            */
            continue
        }

        const { time } = intento.resultado

        sumaLatencia += time

        if (time < result.latenciaMinima)
            result.latenciaMinima = time
        
        if (time > result.latenciaMaxima)
            result.latenciaMaxima = time
    }

    result.latenciaMedia = sumaLatencia / (resultadosConexion.length - result.errores.length)

    return result
}

export async function investig(puerto: number, direccion: string = '127.0.0.1', tiempoEspera: number = 3000): Promise<boolean> {
    const result = await Ping({ direccion, puerto, tiempoEspera, intentos: 1 })

    // Si no hay ningún error, entonces el dispositivo es accesible :]
    return result.errores.length === 0
}