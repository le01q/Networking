import socket

servidorSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

servidorSocket.bind(('localhost', 7777))

(MsjCliente, (IPCliente, PuertoCliente)) = servidorSocket.recvfrom(1000);

servidorSocket.sendto('abc',(IPCliente, PuertoCliente))

print(MsjCliente)

servidorSocket.close()