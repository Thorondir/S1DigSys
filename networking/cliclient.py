import socket, struct

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

buffersize = 1024

while True:
    msg = input()
    msg = int(msg,16).to_bytes(int(len(msg)/2),'big')
    sock.sendto(msg, ("127.0.0.1", 4200))
    received = sock.recv(buffersize)
    try:
        received = struct.unpack('<cii1015x', received)
        print(received)
    except:
        pass
