import socket
import sys

def py_socket(domain, sock_type, protocol):
    print "hello socket"
    s = socket.socket(domain, sock_type, protocol);
    print s.fileno()
    print "bye socket"

    sys.stdout.flush()
    return s.fileno()
    #return -1

