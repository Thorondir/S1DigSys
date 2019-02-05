import socket, sys, curses
from curses import wrapper

# use pygame for multi-char input

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # default socket is has 0 default third value

def main():
    stdscr = curses.initscr()
    curses.cbreak()
    stdscr.keypad(1)
    
    run = True
    
    while run:

        msg = stdscr.getkey()
        

        
        sock.sendto(msg.encode(), ("127.0.0.1", 4200)) # send message to address (tuple with string ip and int port)

    stdscr.keypad(0)
    curses.nocbreak()
    curses.endwin()

wrapper(main)
