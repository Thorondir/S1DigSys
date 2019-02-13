import socket, struct

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
IP = ""
PORT = 4203

def join(iptojoin, name):
    global IP
    IP = iptojoin
    socket.sendto( , (IP, PORT)) # send the server a message telling it to create a new player with name
def leave():
    socket.sendto( , (IP, PORT)) # message telling server to delete this player.
    #INPUTS:  bool{KEY1, KEY2, KEY3, KEY4, UP, DOWN, LEFT, RIGHT}
def send_input(inputs):
    msg = '' 
    for bit in inputs:
        if bit: msg += '1'
        else: msg += '0'
    msg = int(msg,2)
    sock.sendto(bytes([msg]), (IP, PORT))
            
def receive_state():
