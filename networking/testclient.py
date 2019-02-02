import socket, sys

msg = sys.argv[1]

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(msg.encode(), ("127.0.0.1", 4200))

"""
create socket of AF_INET, SOCK_DGRAM, 0?
send command arg to server?
profit?


"""
