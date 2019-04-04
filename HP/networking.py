import socket, struct

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
IP = ""
BUFFERSIZE = 1024
PORT = 4200

def join(iptojoin):
    global IP
    IP = iptojoin
    socket.sendto(bytes([0]) , (IP, PORT)) # send the server a message telling it to create a new player with name
    received = socket.recv(BUFFERSIZE)
    if received[0]:
        return received[1]
    else:
        return -1 #anything less than 0 is an error, since slots are unsigned int
def leave(playerslot):
    socket.sendto(bytes[1, playerslot] , (IP, PORT)) # message telling server to delete this player.
    #INPUTS:  bool{KEY1, KEY2, KEY3, KEY4, UP, DOWN, LEFT, RIGHT}
def send_input(inputs):
    msg = '' 
    for bit in inputs:
        if bit: msg += '1'
        else: msg += '0'
    msg = int(msg,2)
    print(bytes([2,msg]))
    socket.sendto(bytes([0x02, 0, msg]), (IP, PORT))
    received = socket.recv(BUFFERSIZE)
    received = struct.unpack('<cii1015x', received)
    return (received[0], (received[1],received[2]))
        
