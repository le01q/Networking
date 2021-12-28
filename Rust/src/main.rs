extern crate rand;
use rand::{thread_rng, Rng};

use std::io::prelude::*;
use std::env;
use std::net::{TcpListener, TcpStream, Shutdown};

fn main()
{
    let opts: Vec<_> = env::args().collect();
    let defaultServer = "12125";
    let defaultHost = "localhost";
    let defaultPort: i16 = 1;

    if opts.len() <= 1 {
        println!("Ningún argumento especificado por defecto a GPU");
    } 
    else 
    {
        gpus = opts[1].parse().unwrap_or(1);
    }

    println!("Empezando el servidor por {} GPUs", gpus);

    let mut ports: Vec<i16> = Vec::new();
    let mut hosts: Vec<String> = Vec::new();

    for port in defaultPort...(defaultPort + gpus) {
        let host = defaultHost.to_string() + &port.to_string();
        hosts.push(host);
        ports.push(port);
        println!("Se enviará los datos via TCP al puerto: {}", port);
    }

    println!("Empezando el servidor TCP en el puerto {}", defaultServer);
    let server = TcpListener::bind(defaultServer.to_string()).unwrap();

    for mut stream in server.incoming() {
        match server.accept() {
            Ok((_socket, addr)) => println!("Nuevo cliente: {:?}", addr),
            Err(e) => println!("No se puede obtener el cliente: {:?}", e),
        }

        match stream {
            Err(e) => { println!("Peticion malformada {}", e) }
            Ok(mut stream) => {
                let mut buf = Vec::new();
                stream.read_to_end(&mut buf).unwrap();
                println!("Bytes recibidos en total: {:?}", buf);

                if buf[0] != 1 
                {
                    for i in 37..45 {
                        let randomNumber: u8 = thread_rng().gen_range(1, 255);
                        buf[i] = randomNumber;
                    }
                }

                for host in hosts.clone() {
                    let mut stream = TcpStream::connect(host.to_string()).unwrap();
                    stream.write(&buf).unwrap();
                    stream.shutdown(Shutdown::Both).expect("El apagado ha fallado");
                    println!("Bytes enviados: {:?} to {}", buf, host);
                }
            }
        }
    }
}