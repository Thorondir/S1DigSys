import socket, struct, sys, _thread
"""
USE JOIN ONCE. This creates a play object & slot.
When the client exits, use LEAVE so that the slot can be cleared.
use send_input instead of parsing input in client.INPUTS:  bool{KEY1, KEY2, KEY3, KEY4, UP, DOWN, LEFT, RIGHT}
Extra keys can be for other things
Get data might not function very well in game loop right now, but can be used to edit data that will be stored on the server
"""

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
IP = ""
BUFFERSIZE = 1024
PORT = 4200
received = 0

def rcv():
    global received
    while True:
        received = socket.recv(BUFFERSIZE)
        try:
            #number of players
            playernum = struct.unpack('>iiB', received[0:((2*4)+1)])[2]
            received = struct.unpack('>iiB{}i{}x'.format(playernum*2, BUFFERSIZE-(((2+(2*playernum))*4)+1)), received)
        except:
            pass

def join(iptojoin):
    global IP, slot
    IP = iptojoin
    socket.sendto(bytes([0]) , (IP, PORT)) # send the server a message telling it to create a new player with NO name
    received = socket.recv(BUFFERSIZE)
    received = struct.unpack('>BB', received[0:2])
    _thread.start_new_thread(rcv, ())
    if received[0]:
        slot = received[1]
        return 1
    else:
        return -1 #anything less than 0 is an error
def leave():
    global slot
    socket.sendto(bytes[1, slot] , (IP, PORT)) # message telling server to delete this player.
    #INPUTS:  bool{KEY1, KEY2, KEY3, KEY4, UP, DOWN, LEFT, RIGHT}
def send_input(inputs):
    global slot
    msg = '' 
    for bit in inputs:
        if bit: msg += '1'
        else: msg += '0'
    msg = int(msg,2)
    socket.sendto(bytes([0x02, slot, msg]), (IP, PORT))

def get_data(key):#key is a character
    msg = struct.pack('>BBB', 0x3, 0x0, ord(key))
    print(msg)
    socket.sendto(msg, (IP, PORT))
    received = socket.recv(BUFFERSIZE)
    received = struct.unpack('<cii1015x', received)
    return (received[0], (received[1], received[2]))

def receive():
    global received
    return received
