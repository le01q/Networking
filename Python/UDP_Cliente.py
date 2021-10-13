import socket

clienteSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);

clienteSocket.sendto('oi', ('localhost', 7777))

(MsjServidor, (ServidorIP, ServidorPuerto)) = clienteSocket.recvfrom(1000)

print(MsjServidor)

clienteSocket.close()
