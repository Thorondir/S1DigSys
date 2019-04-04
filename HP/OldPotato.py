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

#Finds delay between each frame
getTicksLastFrame = 0

time_1 = pygame.time.get_ticks()
deltatime = (time_1 - getTicksLastFrame)
getTicksLastFrame = time_1



#Player  with x,y cord | speed | velocity | the controls used | control state (pressed or not) | sprite file | size multiplier of original | complete, one time to load sprite | Center of sprite
class player():
	def __init__(self, player, x, y, speed, xvel, yvel, controls, cstate, sprite, size, complete, central): 
		self.player = player
		self.x = x
		self.y = y
		self.size = size
		self.speed = speed
		self.sprite = sprite
		self.controls = controls
		self.xvel = xvel
		self.yvel = yvel
		self.size = size
		self.complete = complete
		self.cstate = cstate
		self.central = central


	def changex (self):
		self.x = self.x + self.xvel*deltatime/1000

	def changey (self):
		self.y = self.y + self.yvel*deltatime/1000

	#One time thing, loads sprite
	def get_image (self):
		if self.complete == 0:
			self.complete = 1
			self.sprite = pygame.image.load(self.sprite)

	def	center (self):
		self.central[0] = (self.x + self.sprite.get_rect().centerx*self.size)
		self.central[1] = (self.y + self.sprite.get_rect().centery*self.size)



class weapon():
	def __init__(self, player, sprite, complete, size, cord, rotation, center1, center2, mousestate, attackspeed):
		self.player = player	
		self.sprite = sprite
		self.size = size
		self.complete = complete
		self.cord = cord
		self.rotation = rotation
		self.center1 = center1
		self.center2 = center2
		self.mousestate = mousestate
		self.attackspeed = attackspeed

	def get_image (self):
		if self.complete == 0:
			self.complete = 1
			self.sprite = pygame.image.load(self.sprite)

	#Weird fked up shete cause u have to find the center of the player and move the the weapon by the distance of the center to the center of the original not rotated sprite and then add it to the rotated which is then moves half backwards in both vectors to spin in the middle
	def find_double_central (self):
		self.cord[0] = (player_list[self.player].central[0] + ((pygame.transform.rotozoom(self.sprite, 0, i.size)).get_rect().centerx - (pygame.transform.rotozoom(self.sprite, i.rotation, i.size)).get_rect().centerx) - self.sprite.get_rect().size[0]/2*self.size) 	                
		self.cord[1] = (player_list[self.player].central[1] + ((pygame.transform.rotozoom(self.sprite, 0, i.size)).get_rect().centery - (pygame.transform.rotozoom(self.sprite, i.rotation, i.size)).get_rect().centery) - self.sprite.get_rect().size[1]/2*self.size) + (player_list[self.player].sprite.get_rect().size[1]*player_list[self.player].size)/6

	def direction(self):
		a = pygame.mouse.get_pos()[0] - player_list[self.player].central[0] #delta x
		b = pygame.mouse.get_pos()[1] - player_list[self.player].central[1] #delta y

		try:
			c = -(m.degrees(m.atan(b/a)))
			works = 1
		except:
			if b > 0:
				self.rotation = 90
			elif b < 0:
				self.rotation = 270
			else:
				self.rotation = 0
			works = 0				

		if works == 1:
			if a > 0:
				self.rotation = 0 + c
			else:
				self.rotation = 180 + c	

	def attack(self):
		print("attack")





#(pygame.transform.scale(self.sprite, (int(

	def test (self):
		return player_list[self.player].central[0] + ((pygame.transforaem.rotozoom(self.sprite, 0, i.size)).get_rect().centerx - (pygame.transform.rotozoom(self.sprite, i.rotation, i.size)).get_rect().centerx)

	def test2 (self):
		return player_list[self.player].central[1] +(pygame.transform.rotozoom(self.sprite, 0, i.size)).get_rect().centery - (pygame.transform.rotozoom(self.sprite, i.rotation, i.size)).get_rect().centery




#Testing
#(int(self.sprite.get_rect().size[0]*self.size), int(self.sprite.get_rect().size[1]*self.size))

player_list = []
weapon_list = []

#Creates player & weapon, probs gonna make a class for it
player_list.append(player(0, 500, 500, 250, 0, 0, [pygame.K_w, pygame.K_a, pygame.K_s, pygame.K_d], [0,0,0,0], "sprites/ninja.png", 1.5, 0, [0,0] ))
weapon_list.append(weapon(0, "sprites/star.png", 0, 1.5, [0,0], 20, 0, 0, 0, 1000))

player_list.append(player(1, 700, 700, 250, 0, 0, [pygame.K_UP, pygame.K_LEFT, pygame.K_DOWN, pygame.K_RIGHT], [0,0,0,0], "sprites/gladiator.png", 1.5, 0, [0,0] ))
weapon_list.append(weapon(1, "sprites/vax.png", 0, 0.3, [0,0], 20, 0, 0, 0, 1000))

f = pygame.font.SysFont("lucida bright", 20)
time = 0


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



		for i in player_list:

			i.get_image()

			if event.type == pygame.KEYDOWN:
				if event.key == i.controls[0]:
					i.cstate[0] = 1


				if event.key == i.controls[2]:
					i.cstate[2] = 1
					
				if event.key == i.controls[3]:
					i.cstate[3] = 1
					
				if event.key == i.controls[1]:
					i.cstate[1] = 1
					

			if event.type == pygame.KEYUP:
				if event.key == i.controls[0]:	
					i.cstate[0] = 0

				if event.key == i.controls[2]:	
					i.cstate[2] = 0

				if event.key == i.controls[1]:	
					i.cstate[1] = 0

				if event.key == i.controls[3]:			
					i.cstate[3] = 0

		for i in weapon_list:
			if event.type == pygame.KEYDOWN:
				if event.key == pygame.K_t:
					print(i.test())
					print(i.test2())


					i.rotation += -10
			if event.type == pygame.MOUSEBUTTONDOWN:
				i.attack()	


	for i in player_list:
		if i.cstate[0] == 1:
			if i.yvel > -i.speed:
				i.yvel += -i.speed*0.1

		if i.cstate[2] == 1:
			if i.yvel < i.speed:
				i.yvel += i.speed*0.1


		if i.cstate[1] == 1:
			if i.xvel > -i.speed:
				i.xvel += -i.speed*0.1
					
			
		if i.cstate[3] == 1:
			if i.xvel < i.speed:
				i.xvel += i.speed*0.1


		if i.cstate[0] == 0 and i.cstate[2] == 0:
			i.yvel = i.yvel * 0.8	

		if i.cstate[1] == 0 and i.cstate[3] == 0:
			i.xvel = i.xvel * 0.8		

		i.changex()
		i.changey()	

		i.center()

		image_1 = pygame.transform.scale(i.sprite, (int(i.sprite.get_rect().size[0]*i.size), int(i.sprite.get_rect().size[1]*i.size)))

		if weapon_list[i.player].rotation > 90:
			image_1 = pygame.transform.flip(image_1, True, False)

		screen.blit(image_1, (i.x,i.y))


	for i in weapon_list:

		i.direction()
		i.get_image()

		i.find_double_central()
		screen.blit(pygame.transform.rotate(pygame.transform.scale(i.sprite, (int(i.sprite.get_rect().size[0]*i.size), int(i.sprite.get_rect().size[1]*i.size))), i.rotation), (i.cord[0], i.cord[1]))

	pygame.display.update()
	screen.fill((255,255,255))