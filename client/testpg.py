import networking, pygame

pygame.init()
screen = pygame.display.set_mode((400,300))

quit = False
networking.join('127.0.0.1')
x = 0
y = 0

while not quit:
    for event in pygame.event.get():
        if event.type==pygame.QUIT:
            quit = True

    pressed = pygame.key.get_pressed()
    networking.send_input([0,0,0,0,pressed[pygame.K_w],pressed[pygame.K_s],pressed[pygame.K_a],pressed[pygame.K_d]])
    recv = networking.receive()
    print(recv)
    if recv != 0:
        x = recv[0]
        y = recv[1]

    screen.fill((0,0,0))
    pygame.draw.rect(screen, (0,0,255), pygame.Rect(x,y,10,10))

    pygame.display.flip()
