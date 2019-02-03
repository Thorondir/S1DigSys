import socket, sys

msg = sys.argv[1]

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # default socket is has 0 default third value
sock.sendto(msg.encode(), ("127.0.0.1", 4200)) # send message to address (tuple with string ip and int port)

