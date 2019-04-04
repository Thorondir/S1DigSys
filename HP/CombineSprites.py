import time
import random
import pygame
import os
import math as m

show = (0,30)
hide = (1000,1000)


#Initializes screen and center
pygame.init()
display_res = pygame.display.Info()
print(display_res)
os.environ['SDL_VIDEO_WINDOW_POS'] = "%d,%d" % show

#Creates screen with given own display resolution, not resizeable or it will break
screen = pygame.display.set_mode((int(display_res.current_w), int(display_res.current_h-30)))
screen.fill((255, 255, 255))
pygame.display.update()
crashed = False

getTicksLastFrame = 0

time_1 = pygame.time.get_ticks()
deltatime = (time_1 - getTicksLastFrame)
getTicksLastFrame = time_1

def load(x):
	return pygame.image.load(x).convert_alpha()

potato = load("sprites/ninja.png")
potato1 = load("sprites/back_1.png")		
potato2 = load("sprites/back_2.png")	

test_surface = pygame.Surface((int(display_res.current_w), int(display_res.current_h-30)), pygame.SRCALPHA)
a = -potato1.get_rect().size[0]	
b = 0
yy = 0


while not crashed:

	time_1 = pygame.time.get_ticks()
	deltatime = (time_1 - getTicksLastFrame)
	getTicksLastFrame = time_1 


	#screen.blit(f.render("Time = " + str(time), 1, (0,0,0)), (10,10))

	for event in pygame.event.get():

		if event.type == pygame.QUIT:
			crashed = True

		if event.type == pygame.KEYDOWN:
			if event.key == pygame.K_p:
				crashed = True	

	
	for i in range(0,50):
		for i in range(0,51):
			a += potato1.get_rect().size[0]	
			b = random.randint(0,1)
			if b == 0:
				test_surface.blit(potato1,(a,yy))
			else:
				test_surface.blit(potato2,(a,yy))

		yy += potato1.get_rect().size[1]	
		a = -potato1.get_rect().size[0]


	


		
	


	screen.blit(test_surface, [0,0])


	pygame.display.update()
	screen.fill((255,255,255))