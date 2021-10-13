require('socket');

io.write("[NW] > Escoge el servidor: ");

servidor = io.read();

io.write("[NW] > Selecciona un puerto: ")

puerto = io.read();

cliente = socket.connect(servidor, puerto);

if cliente then
    io.write("Te has conectado correctamente!");

    while True do
        io.write("Envia un mensaje: ");
        cliente:send(io.read() .. "\n");
        respuesta = cliente:receive();
        io.write(respuesta .. "\n")
    end
end