require('socket');

puerto = 80;

servidor = socket.bind('*', puerto);

io.write("[NW] > Esperando en la entrada: "..puerto.."\n");

con = servidor:accept();

io.write("[NW] > Conexion desde: ");

while True do
    mensaje = con:receive();
    io.write(mensaje .. "\n");
    io.write("[NW] > Tu respuesta: ");
    con:send(io.read() .. "\n");
end 

io.read();
