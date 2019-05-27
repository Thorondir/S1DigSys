import socket, struct, _thread

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

buffersize = 1024

def cli():
    global received
    while True:
        msg = input()
        if msg == 'print':
            print(received)
        else:
            msg = int(msg,16).to_bytes(int(len(msg)/2),'big')
            sock.sendto(msg, ("127.0.0.1", 4200))
            print(msg)


def rcv():
    global received
    while True:
        received = sock.recv(buffersize)
        try:
            #number of players
            playernum = struct.unpack('>iiB', received[0:((2*4)+1)])[2]
            received = struct.unpack('>iiB{}i{}x'.format(playernum*2, buffersize-(((2+(2*playernum))*4)+1)), received)
        except:
            print("strange output")
            pass

_thread.start_new_thread(cli, ())
_thread.start_new_thread(rcv, ())

while True:
    pass
