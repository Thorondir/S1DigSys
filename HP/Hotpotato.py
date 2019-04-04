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
screen = pygame.display.set_mode((int(display_res.current_w), int(display_res.current_h)))
screen.fill((255, 255, 255))
pygame.display.update()
crashed = False

print(display_res.current_w)
print(display_res.current_h)

#Finds delay between each frame
getTicksLastFrame = 0

time_1 = pygame.time.get_ticks()
deltatime = (time_1 - getTicksLastFrame)
getTicksLastFrame = time_1

player_slot = [0 for x in range(100)]

player_list = []

controls_list = [[pygame.K_w, pygame.K_a, pygame.K_s, pygame.K_d, pygame.K_SPACE],   [pygame.K_UP, pygame.K_LEFT, pygame.K_DOWN, pygame.K_RIGHT, pygame.K_KP_ENTER],   [pygame.K_o, pygame.K_k, pygame.K_l, pygame.K_SEMICOLON]]

spawn_list = [[200,200],[400,400],[600,600],[800,800]]

update_coord_list = []
update_coord_list2 = []

oldcoord = []
oldcoord2 = []

#Loads images
def load(x):
	return pygame.image.load(x).convert_alpha()


# Creates background by compiling images
back_1 = load("sprites/back_1.png")		
back_2 = load("sprites/back_2.png")	

background_surface = pygame.Surface((int(display_res.current_w), int(display_res.current_h)), pygame.SRCALPHA)
tile_size = back_1.get_rect().size

backgroundx = -tile_size[0]
backgroundy = 0

def background_row():
	global backgroundx
	global tile_size

	for i in range(0,43):
			backgroundx += tile_size[0]
			b = random.randint(0,1)
			if b == 0:
				background_surface.blit(back_1,(backgroundx, backgroundy))
			else:
				background_surface.blit(back_2,(backgroundx, backgroundy))


for i in range(0,24):
		background_row()
		backgroundy += tile_size[1]	
		backgroundx = -tile_size[0]


screen.blit(background_surface, [0,0])
pygame.display.update()


#Classes list Array meaning |1st (sprite#1) 2nd (sprite#1 size) 3rd (sprite#2) 4th (sprite#2 size) 5th (speed) 6th (attack speed) 7th (health) 8th (stamina) 9th (sprite#3) 10th (sprite#3 size)

classes_list = [[load("sprites/ninja.png"), 1.5, load("sprites/star.png"), 1.5, 250, 1000, 40, 40], [load("sprites/gladiator.png"), 1.5, load("sprites/vax.png"), 0.3, 250, 1000, 40, 40]]


#All info for new player
class newplayer():
	def __init__(self, player, spawn, controls, classification):
		self.player = player
		self.spawn = spawn
		self.controls = controls
		self.classification = classification

		x = self.spawn[0]
		y = self.spawn[1]

		centerx = y + self.classification[0].get_rect().centerx*self.classification[1]
		centery = x + self.classification[0].get_rect().centery*self.classification[1]
	

		self.playerinfo = self.playerinfo(self.player, [x, y], [centerx, centery], self.controls, [0,0,0,0], 0, self.classification[4], [0,0], self.classification[0], self.classification[1], self.classification, [])

	class playerinfo():	
		def __init__(self, player, coordinates, central, controls, cstate, rotation, speed, velocity, sprite, size, classification, iginfo):

			self.player = player 
			self.coordinates = coordinates
			self.central = central
			self.controls = controls
			self.cstate = cstate
			self.rotation = rotation
			self.speed = speed
			self.velocity = velocity
			self.sprite = sprite
			self.size = size 
			self.classification = classification
			self.iginfo = iginfo


			#Finds the points for the weapons to blit, combines the central coordinates, rotation (which offsets the coords) & also moves these coordianates so that the sprite's central is on the player
			ocenterx = pygame.transform.rotozoom(self.sprite, 0, self.size).get_rect().centerx #Finds center of player original
			rcenterx = pygame.transform.rotozoom(self.sprite, self.rotation, self.size).get_rect().centerx #Finds center of player when rotated
			movehalfx = self.sprite.get_rect().size[0]/2*self.size # moves back to center as image is printed from top left 

			ocentery = pygame.transform.rotozoom(self.sprite, 0, self.size).get_rect().centery #Same as above for y coord
			rcentery = pygame.transform.rotozoom(self.sprite, self.rotation, self.size).get_rect().centery #Same as above for y coord
			movehalfy = self.sprite.get_rect().size[1]/2*self.size #Same as above for y coord

			ccenterx = self.central[0] + ocenterx - rcenterx - movehalfx #Center coord - difference between original and after rotated, moves back to center
			ccentery = self.central[1] + ocentery - rcentery - movehalfy



			self.weapon = self.weapon(self.player, self.classification[2], self.classification[3], [ccenterx, ccentery], 0, 0, self.classification[5], self.classification)

		def changex (self):
			self.coordinates[0] = self.coordinates[0] + self.velocity[0]*deltatime/1000 #Sets new bliting coordinates to old + the velocity

		def changey (self):
			self.coordinates[1] = self.coordinates[1] + self.velocity[1]*deltatime/1000	

		def newcenter(self):
			self.central[0] = self.coordinates[0] + self.sprite.get_rect().centerx*self.size #Sets new center 
			self.central[1] = self.coordinates[1] + self.sprite.get_rect().centery*self.size


		class weapon():	
			def __init__(self, player, sprite, size, coordinates, rotation, mousestate, attackspeed, classification):
				self.player = player	
				self.sprite = sprite
				self.size = size
				self.coordinates = coordinates
				self.rotation = rotation
				self.mousestate = mousestate
				self.attackspeed = attackspeed
				self.classification = classification

			def find_double_central (self):
				self.coordinates[0] = (i.playerinfo.central[0] + ((pygame.transform.rotozoom(self.sprite, 0, self.size)).get_rect().centerx - (pygame.transform.rotozoom(self.sprite, self.rotation, self.size)).get_rect().centerx) - self.sprite.get_rect().size[0]/2*self.size)               
				self.coordinates[1] = (i.playerinfo.central[1] + ((pygame.transform.rotozoom(self.sprite, 0, self.size)).get_rect().centery - (pygame.transform.rotozoom(self.sprite, self.rotation, self.size)).get_rect().centery) - self.sprite.get_rect().size[1]/2*self.size) + 10 	

  
			def direction(self):
				a = pygame.mouse.get_pos()[0] - player_list[self.player].playerinfo.central[0] #delta x
				b = pygame.mouse.get_pos()[1] - player_list[self.player].playerinfo.central[1] #delta y

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

			def attack():
				print("attack")		

			class projectile():
				def __init__(self, player, sprite, size, coordinates, rotation, mousestate, speed, classification):
					self.player = player	
					self.sprite = sprite
					self.size = size
					self.coordinates = coordinates
					self.rotation = rotation
					self.mousestate = mousestate
					self.speed = speed
					self.classification = classification

		class abilities():
			pass




def createplayer(classification):

	player_num = 0

	for i in range(0,len(player_slot)):

		if player_slot[i] == 0:
			player_num = i
			player_slot[i] = 1
			break	

	player_list.append(newplayer(player_num, spawn_list[player_num], controls_list[player_num], classes_list[classification]))	

testv = 100
for i in range(0,0):
	testv += 10
	spawn_list.append([testv, 0])
	controls_list.append([pygame.K_w, pygame.K_a, pygame.K_s, pygame.K_d, pygame.K_SPACE])
	createplayer(0)
	


createplayer(0)
createplayer(1)

print(player_list[0].playerinfo.central)

player_list[0].playerinfo.coordinates[0] += 100

player_list[0].playerinfo.newcenter()

print(player_list[0].playerinfo.central)

			


#pygame.time.set_timer(pygame.USEREVENT +0, 1000)
#fps_counter = 1000/deltatime

#pygame.time.set_timer(1, 50)
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
	
				

		#if event.type == pygame.USEREVENT:
			#pygame.time.set_timer(pygame.USEREVENT, 1000)	
			#fps_counter = 1000/deltatime
				



		for i in player_list:

			if event.type == pygame.KEYDOWN:
				if event.key == i.playerinfo.controls[0]:
					i.playerinfo.cstate[0] = 1

				if event.key == i.playerinfo.controls[2]:
					i.playerinfo.cstate[2] = 1
					
				if event.key == i.playerinfo.controls[3]:
					i.playerinfo.cstate[3] = 1
					
				if event.key == i.playerinfo.controls[1]:
					i.playerinfo.cstate[1] = 1

				#Basic attack
				#if event.key == i.playerinfo.controls[4]:
				#	i.playerinfo.weapon.attack()
						
					

			if event.type == pygame.KEYUP:
				if event.key == i.playerinfo.controls[0]:	
					i.playerinfo.cstate[0] = 0

				if event.key == i.playerinfo.controls[2]:	
					i.playerinfo.cstate[2] = 0

				if event.key == i.playerinfo.controls[1]:	
					i.playerinfo.cstate[1] = 0

				if event.key == i.playerinfo.controls[3]:			
					i.playerinfo.cstate[3] = 0



	for i in player_list:

		if i.playerinfo.cstate[0] == 1:
			if i.playerinfo.velocity[1]> -i.playerinfo.speed:
				i.playerinfo.velocity[1]+= -i.playerinfo.speed*0.1

		if i.playerinfo.cstate[2] == 1:
			if i.playerinfo.velocity[1]< i.playerinfo.speed:
				i.playerinfo.velocity[1]+= i.playerinfo.speed*0.1		

		if i.playerinfo.cstate[1] == 1:
			if i.playerinfo.velocity[0]> -i.playerinfo.speed:
				i.playerinfo.velocity[0]+= -i.playerinfo.speed*0.1

		if i.playerinfo.cstate[3] == 1:
			if i.playerinfo.velocity[0]< i.playerinfo.speed:
				i.playerinfo.velocity[0]+= i.playerinfo.speed*0.1	


		if i.playerinfo.cstate[0] == 0 and i.playerinfo.cstate[2] == 0:
			i.playerinfo.velocity[1] = 	i.playerinfo.velocity[1]*0.8	

		
		if i.playerinfo.cstate[1] == 0 and i.playerinfo.cstate[3] == 0:
			i.playerinfo.velocity[0] = 	i.playerinfo.velocity[0]*0.8				


		i.playerinfo.changex()
		i.playerinfo.changey()	
		i.playerinfo.weapon.direction()

		i.playerinfo.newcenter() 
		i.playerinfo.weapon.find_double_central()
	
	extrac = 5
		
	for i in player_list:

		if i.playerinfo.weapon.rotation > 90:
			image_1 = pygame.transform.flip((pygame.transform.scale(i.playerinfo.sprite, (int(i.playerinfo.sprite.get_rect().size[0]*i.playerinfo.size), int(i.playerinfo.sprite.get_rect().size[1]*i.playerinfo.size)))), True, False)
			i.playerinfo.iginfo = [i.playerinfo.coordinates[0]-extrac,i.playerinfo.coordinates[1]-extrac,image_1.get_rect().size[0]+extrac*2,image_1.get_rect().size[0]+extrac*2]
			
			screen.blit(image_1, i.playerinfo.coordinates)
		else:
			image_1 = pygame.transform.scale(i.playerinfo.sprite, (int(i.playerinfo.sprite.get_rect().size[0]*i.playerinfo.size), int(i.playerinfo.sprite.get_rect().size[1]*i.playerinfo.size)))
			i.playerinfo.iginfo = [i.playerinfo.coordinates[0]-extrac,i.playerinfo.coordinates[1]-extrac,image_1.get_rect().size[0]+extrac*2,image_1.get_rect().size[0]+extrac*2]
			
			screen.blit(image_1, i.playerinfo.coordinates)

		update_coord_list.append(i.playerinfo.iginfo)	

		image_1 = pygame.transform.rotozoom(i.playerinfo.weapon.sprite, i.playerinfo.weapon.rotation, i.playerinfo.weapon.size)

		screen.blit(image_1, (i.playerinfo.weapon.coordinates[0],i.playerinfo.weapon.coordinates[1])) #+ i.playerinfo.sprite.get_rect().size[1]*0.2))
		
		update_coord_list.append([i.playerinfo.weapon.coordinates[0]-extrac,i.playerinfo.weapon.coordinates[1]-extrac,image_1.get_rect().size[0]+extrac*2,image_1.get_rect().size[1]+extrac*2])


	
					
	pygame.display.update(update_coord_list)
	pygame.display.update(update_coord_list2)


	screen.blit(background_surface, [0,0])
	update_coord_list2 = update_coord_list
	update_coord_list = []	
			

		

		












		#Testing with rect size of sprites

		#image_1 = i.playerinfo.sprite
#
		#if i.playerinfo.weapon.rotation > 90:
#
			#image_1 = pygame.transform.flip(image_1, True, False)
#
		#image_1 = pygame.transform.scale(image_1, (int(i.playerinfo.sprite.get_rect().size[0]*i.playerinfo.size), int(i.playerinfo.sprite.get_rect().size[1]*i.playerinfo.size)))
#
		#pygame.draw.rect(screen, [200,100,200], ([i.playerinfo.coordinates[0], i.playerinfo.coordinates[1], image_1.get_rect().size[0], image_1.get_rect().size[1]]))	
#
		#screen.blit(image_1, (i.playerinfo.coordinates[0],i.playerinfo.coordinates[1]))
#
		#update_coord_list.append([i.playerinfo.coordinates[0], i.playerinfo.coordinates[1], image_1.get_rect().size[0], image_1.get_rect().size[1]])
#
		#image_1 = pygame.transform.rotozoom(i.playerinfo.weapon.sprite, i.playerinfo.weapon.rotation, i.playerinfo.weapon.size)
#
		#screen.blit(image_1, (i.playerinfo.weapon.coordinates[0],i.playerinfo.weapon.coordinates[1])) #+ i.playerinfo.sprite.get_rect().size[1]*0.2))
#
		#update_coord_list.append([i.playerinfo.weapon.coordinates[0], i.playerinfo.weapon.coordinates[1], pygame.transform.rotozoom(i.playerinfo.weapon.sprite, i.playerinfo.weapon.rotation, i.playerinfo.weapon.size).get_rect().size[0], pygame.transform.rotozoom(i.playerinfo.weapon.sprite, i.playerinfo.weapon.rotation, i.playerinfo.weapon.size).get_rect().size[1]])
#	


		#pygame.draw.rect(screen, [200,200,200], update_coord_list[1])

		#playerhit = pygame.Surface((pygame.transform.rotozoom(i.playerinfo.sprite, i.playerinfo.rotation, i.playerinfo.size).get_rect().size[0],pygame.transform.rotozoom(i.playerinfo.sprite, i.playerinfo.rotation, i.playerinfo.size).get_rect().size[1]), pygame.SRCALPHA)
		
		#playerhit.fill((255,255,255,128))
		#screen.blit(playerhit, (i.playerinfo.coordinates))


		#pygame.draw.rect(screen, [220,100,100],(i.playerinfo.coordinates[0], i.playerinfo.coordinates[1], pygame.transform.rotozoom(i.playerinfo.sprite, i.playerinfo.rotation, i.playerinfo.size).get_rect().size[0], pygame.transform.rotozoom(i.playerinfo.sprite, i.playerinfo.rotation, i.playerinfo.size).get_rect().size[1]))

		#pygame.draw.rect(screen, [200,200,200], (i.playerinfo.weapon.coordinates[0],i.playerinfo.weapon.coordinates[1],pygame.transform.rotozoom(i.playerinfo.weapon.sprite, i.playerinfo.weapon.rotation, i.playerinfo.weapon.size).get_rect().size[0],pygame.transform.rotozoom(i.playerinfo.weapon.sprite, i.playerinfo.weapon.rotation, i.playerinfo.weapon.size).get_rect().size[1]))


		#Get rect of actual rotated sprite	pygame.transform.rotozoom(self.sprite, 0, self.size)).get_rect().x & y <------ try this later


	#for i in update_coord_list:
	#	pygame.display.update(i)

		#screen.blit(pygame.transform.rotate(pygame.transform.scale(i.sprite, (int(i.sprite.get_rect().size[0]*i.size), int(i.sprite.get_rect().size[1]*i.size))), i.rotation), (i.cord[0], i.cord[1]))

	
	
		#screen.blit(pygame.font.SysFont("" , 30).render("FPS = " + str(int(fps_counter)), 1, (0,0,0)), (10,10))	

	
	#screen.fill([255,255,255])
			

		#if keys[pygame.K_(player_list[1].right)] and not keys[pygame.K_(player_list[1].left)]:
		#	player_list[i].changex()
		#	print (player_list[i].x)


		#if keys[pygame.K_a] and not keys[pygame.K_d]:
		#	player_list[i].changexn()
		#	print (player_list[i].x)		


#class player():
#	def __init__(self, colour, size):
#		self.colour = colour
#		self.size = size
#
#	def colourcha(self, colour):
#		self.colour = colour
#
#	def sizecha(self, size):
#		self.size = size
#
#	def combine(self):
#		return '{} hi {}'.format(self.size, self.colour)
#
#potato = player("red", 100)
#print (potato.colour)
#potato.colourcha("blue")
#potato.sizecha(200)

#print(potato.combine())
