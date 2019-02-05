import socket, sys, pygame

# use pygame for multi-char input
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # default socket is has 0 default third value

pygame.init()
screen = pygame.display.set_mode((150,150))

run = True
dirkeys = [False, False, False, False]
while run:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:        dirkeys[0] = True
            elif event.key == pygame.K_DOWN:    dirkeys[1] = True
            elif event.key == pygame.K_LEFT:    dirkeys[2] = True
            elif event.key == pygame.K_RIGHT:   dirkeys[3] = True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_UP:        dirkeys[0] = False
            elif event.key == pygame.K_DOWN:    dirkeys[1] = False
            elif event.key == pygame.K_LEFT:    dirkeys[2] = False
            elif event.key == pygame.K_RIGHT:   dirkeys[3] = False
    
    msg = '0000'
    
    for bit in dirkeys:
        if bit:
            msg += '1'
        else:
            msg += '0'
    
    msg = int(msg, 2)
    msg = str(msg).encode()

    sock.sendto(msg, ("127.0.0.1", 4200)) # send message to address (tuple with string ip and int port)

