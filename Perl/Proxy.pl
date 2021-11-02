# TCP PROXY POR le01q

use advertencia;
use estricto;

use IO::Socket;
use IO::Select;

my @ip_validas = ('1.2.3.4', '5.6.7.8', '127.0.0.1', '192.168.1.2');
my $ioset = IO::Select -> new;
my %socket_map;

my $debug = 1;

sub Nueva_Conexion {
    my $servidor = shift;
    my $cliente = $servidor->accept;
    my $ip_cliente = ip_cliente($cliente);

    unless (clienteValido($cliente)) {
        print "Conexion desde $ip_cliente denegado\n" if $debug;
        $cliente->close;
        return;
    }
    print "Conexion desde $ip_cliente consedido!\n" if $debug;

    my $remote = new_conn('localhost', 55555);
    $ioset->add($cliente);
    $ioset->add($remote);

    $socket_map{$cliente} = $remote;
    $socket_map{$remote} = $cliente;
}

sub CerrarConexion {
    my $cliente = shift;
    my $ip_cliente = ip_cliente($cliente);
    my $remote = $socket_map{$cliente};
    
    $ioset->remove($cliente);
    $ioset->remove($remote);

    delete $socket_map{$cliente};
    delete $socket_map{$remote};

    $cliente->close;
    $remote->close;

    print "Conexion desde $ip_cliente cerrada\n" if $debug;
}

sub ip_cliente {
    my $cliente = shift;
    return inet_ntoa($cliente->sockaddr);
}

sub clienteValido {
    my $cliente = shift;
    my $ip_cliente = ip_cliente($cliente);
    return grep { $_ eq $ip_cliente } @allowed_ips;
}

print "Alojado en: 0.0.0.0:1080\n";
my $servidor = new_server('0.0.0.0', 1080);
$ioset->add($servidor);

while (1) {
    for my $socket ($ioset->can_read) {
        if ($socket == $servidor) {
            new_connection($servidor);
        }
        else {
            next unless exists $socket_map{$socket};
            my $remote = $socket_map{$socket};
            my $buffer;
            my $read = $socket->sysread($buffer, 4096);
            if ($read) {
                $remote->syswrite($buffer);
            }
            else {
                CerrarConexion($socket);
            }
        }
    }
}