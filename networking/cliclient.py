import socket, struct

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

buffersize = 1024

while True:
    msg = input()
    if msg == "22":
        sock.sendto(bytes([0x01,0x00]), ("127.0.0.1", 4200))
    else:
        msg = int(msg,2)
        sock.sendto(bytes([msg]), ("127.0.0.1", 4200))

    received = sock.recv(buffersize);
    
    print(received[0])
