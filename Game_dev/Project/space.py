""" 
Sergio Avila

In this program I will implement a versin of the  game space invaders
for Game Development class. In this game there will be a main ship where 
it will be able to shoot bullets to other smaller space ships.

This project will be done in python and 
it will be ran through the terminal/command prompt.

For this program to run you will have to have python installed
in your computer. However, if you use ubuntu python comes 
already installed.
Also you will need to install the pygame library from pygame.org

How to run it:
First go to your folder where search.py is in.
For example: cd /Game-Dev/Project$ 
once you are there you simply type:
python search.py
"""
import pygame
from pygame import *
import sys
from random import shuffle, randrange, choice

#// globals

SCREEN = display.set_mode((800,600))
PICNAME = ["motherShip","bullet", "enemy_A", "enemy_B", "enemy_C", "explosion_all", "boss_fight"]
PICTURES = {name: image.load("images/{}.png".format(name)).convert_alpha() for name in PICNAME}

WHITE 	= (255, 255, 255)
GREEN 	= (78, 255, 87)
YELLOW 	= (241, 255, 0)
BLUE 	= (80, 255, 239)
PURPLE 	= (203, 0, 255)
RED 	= (237, 28, 36)
FONTS = "fonts/game_font_7.ttf"


class MotherShip(sprite.Sprite):
	def __init__(self):
		sprite.Sprite.__init__(self)
		self.image = PICTURES["motherShip"]
		self.rect = self.image.get_rect(topleft=(375,540))
		self.speed = 5

	def update(self, keys, *args):
		if keys[K_LEFT] and self.rect.x > 10:
			self.rect.x -= self.speed
		if keys[K_RIGHT] and self.rect.x < 740:
			self.rect.x += self.speed

		game.screen.blit(self.image, self.rect)

class Bullet(sprite.Sprite):
	def __init__(self, x, y, direction, speed, filename, side):
		sprite.Sprite.__init__(self)
		self.image = PICTURES[filename]
		self.rect = self.image.get_rect(topleft=(x, y))
		self.speed = speed
		self.direction = direction
		self.side = side
		self.filename = filename

	def update(self, keys, *args):
		game.screen.blit(self.image, self.rect)
		self.rect.y += self.speed * self.direction
		if self.rect.y < 15 or self.rect.y > 600:
			self.kill()

class Block(sprite.Sprite):

	def __init__(self,size,color, row, column):
		sprite.Sprite.__init__(self)
		self.height = size
		self.width = size
		self.color = color
		self.image = Surface((self.width, self.height))
		self.image.fill(self.color)
		self.rect = self.image.get_rect()
		self.row = row
		self.column = column

	def update(self, keys, *args):
		game.screen.blit(self.image, self.rect)

class boss_fight(sprite.Sprite):
	def __init__(self):
		sprite.Sprite.__init__(self)
		self.image = PICTURES["boss_fight"]
		self.image = transform.scale(self.image, (75, 35))
		self.rect = self.image.get_rect(topleft=(-80, 45))
		self.row = 5
		self.moveTime = 25000
		self.direction = 1
		self.timer = time.get_ticks()

	def update(self, keys, currentTime, *args):
		resetTimer = False
		if (currentTime - self.timer > self.moveTime) and self.rect.x < 840 and self.direction == 1:
			self.rect.x += 2
			game.screen.blit(self.image, self.rect)
		if (currentTime - self.timer > self.moveTime) and self.rect.x > -100 and self.direction == -1:
			self.rect.x -= 2
			game.screen.blit(self.image, self.rect)
		if (self.rect.x > 830):
			self.direction = -1  
		if (self.rect.x < -90):
			self.direction = 1
			resetTimer = True
		if (currentTime - self.timer > self.moveTime) and resetTimer:
			self.timer = currentTime


class Collision(sprite.Sprite):
	def __init__(self, x, y, row, ship, boss, score):
		sprite.Sprite.__init__(self)
		self.isMothership = ship
		self.isBoss = boss
		if boss:
			self.text = Text(FONTS, 20, str(score), WHITE, x+20, y+6)
		elif ship:
			self.image = PICTURES["motherShip"]
			self.rect = self.image.get_rect(topleft=(x,y))
		else:
			self.row = row
			self.load_image()
			self.image = transform.scale(self.image, (40, 35))
			self.rect = self.image.get_rect(topleft=(x,y))
			game.screen.blit(self.image, self.rect)

		self.timer = time.get_ticks()

	def update(self, keys, currentTime):
		if self.isBoss:
			if currentTime - self.timer <= 200:
				self.text.draw(game.screen)
			if currentTime - self.timer > 400 and currentTime - self.timer <= 600:
				self.text.draw(game.screen)
			if currentTime - self.timer > 600:
				self.kill()

		if self.isMothership:
			if currentTime - self.timer > 300 and currentTime - self.timer <= 600:
				game.screen.blit(self.image, self.rect)
			if currentTime - self.timer > 900:
				self.kill()

		else:
			if currentTime - self.timer <= 100:
				game.screen.blit(self.image, self.rect)
			if currentTime - self.timer > 100 and currentTime - self.timer <= 200:
				#self.image - transform.scale(self.image, (50, 45))
				game.screen.blit(self.image, (self.rect.x-6, self.rect.y-6))
			if currentTime - self.timer > 400:
				self.kill()

	def load_image(self):
		imgColors = ["_all", "_all", "_all", "_all", "_all"]
		self.image = PICTURES["explosion{}".format(imgColors[self.row])]

	
class Text(object):
	def __init__(self, textFont, size, message, color, xpos, ypos):
		pygame.font.init()
		self.font = font.Font(textFont, size)
		self.surface = self.font.render(message, True, color)
		self.rect = self.surface.get_rect(topleft=(xpos, ypos))

	def draw(self, surface):
		surface.blit(self.surface, self.rect)

class Enemies(sprite.Sprite):

	def __init__(self, row, column):
		sprite.Sprite.__init__(self)
		self.row = row
		self.column = column
		self.images = []
		self.load_images()
		self.index = 0
		self.image = self.images[self.index]
		self.rect = self.image.get_rect()
		self.direction = 1
		self.leftMoves = 30
		self.rightMoves = 15
		self.moveTime = 600
		self.moveNumber = 0
		self.firstTime = True
		self.movedY = False
		self.columns = [False] * 10
		self.aliveColumns = [True] * 10
		self.addRightMoves = False
		self.addLeftMoves = False
		self.numOfRightMoves = 0
		self.numOfLeftMoves = 0
		self.timer = time.get_ticks()

	def update(self, keys, currentTime, deadRow, deadColumn, deadArray):
		self.check_column_deletion(deadRow, deadColumn, deadArray)
		if currentTime - self.timer > self.moveTime:
			self.movedY = False
			if self.moveNumber >= self.rightMoves and self.direction == -1:
				self.direction *= -1
				self.moveNumber = 0
				self.rect.y += 35
				self.movedY = True
				if self.addRightMoves:
					self.rightMoves += self.numOfRightMoves
				if self.firstTime:
					self.rightMoves = self.leftMoves
					self.firstTime = False
				self.addRightMovesAfterDrop = False
			
			if self.moveNumber < self.rightMoves and self.direction == 1 and not self.movedY:
				self.rect.x += 10
				self.moveNumber += 1
			elif self.moveNumber < self.leftMoves and self.direction == 1 and not self.movedY:
				self.rect.x -= 10
				self.moveNumber += 1

			self.index += 1
			if self.index >= len(self.images):
				self.index = 0
			self.image = self.images[self.index]

			self.timer += self.moveTime
		game.screen.blit(self.image, self.rect)

	def check_column_deletion(self, deadRow, deadColumn, deadArray):
		if deadRow != -1 and deadColumn != -1:
			deadArray[deadRow][deadColumn] = 1
			for column in range(10):
				if all([deadArray[row][column] == 1 for row in range(5)]):
					self.columns[column] = True

		for i in range(5):
			if all([self.columns[x] for x in range(i + 1)]) and self.aliveColumns[i]:
				self.leftMoves += 5
				self.aliveColumns[i] = False
				if self.direction == -1:
					self.rightMoves += 5
				else:
					self.addRightMoves = True
					self.addLeftMoves += 5

		for i in range(5):
			if all([self.columns[x] for x in range(9, 8 - i, -1)]) and self.aliveColumns[9 - i]:
				self.aliveColumns[9 - i] = False
				self.rightMoves += 5
				if self.direction == 1:
					self.leftMoves += 5
				else:
					self.addLeftMoves = True
					self.numOfLeftMoves += 5


	def load_images(self):
		images = {0: ["_A", "_A"],
				  1: ["_B", "_B"],
				  2: ["_C", "_C"],
				  3: ["_C", "_C"],
				  4: ["_A", "_A"],
				 }
		img1, img2 = (PICTURES["enemy{}".format(img_num)] for img_num in images[self.row])
		self.images.append(transform.scale(img1, (40, 35)))
		self.images.append(transform.scale(img2, (40, 35)))


class LIVES(sprite.Sprite):
	def __init__(self, x, y):
		sprite.Sprite.__init__(self)
		self.image = PICTURES["motherShip"]
		self.image = transform.scale(self.image, (23, 23))
		self.rect = self.image.get_rect(topleft=(x, y))
		
	def update(self, keys, *args):
		game.screen.blit(self.image, self.rect)


class StartGame(object):
	def __init__(self):
		init()
		self.screen = SCREEN
		self.background = image.load('images/space.jpg').convert()
		self.startGame = False
		self.mainScreen = True
		self.gameOver = False
		self.enemyposition = 65

	def reset(self, score, lives):
		self.player = MotherShip()
		self.playerGroup = sprite.Group(self.player)
		self.explosionsGroup = sprite.Group()
		self.bullets = sprite.Group()
		self.bossShip = boss_fight() 
		self.bossGroup = sprite.Group(self.bossShip)
		self.enemyBullets = sprite.Group()
		self.make_lives()
		self.make_enemies()
		self.allBlockers = sprite.Group(self.make_blockers(0), self.make_blockers(1), self.make_blockers(2), self.make_blockers(3))
		self.keys = key.get_pressed()
		self.clock = time.Clock()
		self.timer = time.get_ticks()
		self.noteTimer = time.get_ticks()
		self.shipTimer = time.get_ticks()
		self.score = score
		self.lives = lives
		self.make_text()
		self.deadRow = -1
		self.deadColumn = -1
		self.makeNewShip = False
		self.shipAlive = True
		self.deadArray = [[0] * 10 for x in range(5)]


	def make_lives(self):
		self.life1 = LIVES(715, 3)
		self.life2 = LIVES(742, 3)
		self.life3 = LIVES(769, 3)
		self.livesGroup = sprite.Group(self.life1, self.life2, self.life3)

	def make_blockers(self, number):
	   blockerGroup = sprite.Group()
	   for row in range(4):
		   for column in range(9):
			   blocker = Block(10, GREEN, row, column)
			   blocker.rect.x = 50 + (200 * number) + (column * blocker.width)
			   blocker.rect.y = 450 + (row * blocker.height)
			   blockerGroup.add(blocker)
	   return blockerGroup


	def make_text(self):
		self.title = Text(FONTS, 50, "Game Dev Project", WHITE, 164, 155)
		self.gameOverText = Text(FONTS, 50, "Game Over", WHITE, 250, 270)
		self.nextRound = Text(FONTS, 50, "Next Round", WHITE, 240, 270)
		self.enemyA = Text(FONTS, 25, "   =   10 pts", GREEN, 368, 270)
		self.enemyC = Text(FONTS, 25, "   =  20 pts", PURPLE, 368, 370)
		self.enemyB = Text(FONTS, 25, "   =  30 pts", BLUE, 368, 320)
		self.enemyBoss = Text(FONTS, 25, "   =  ?????", RED, 368, 420)
		self.scoreText = Text(FONTS, 20, "Score", WHITE, 5, 5)
		self.livesText = Text(FONTS, 20, "Lives ", WHITE, 640, 5)

	def userInput(self):
		self.keys = key.get_pressed()
		for i in event.get():
			if i.type == QUIT:
				sys.exit()
			if i.type == KEYDOWN:
				if i.key == K_SPACE:
					if len(self.bullets) == 0 and self.shipAlive:
						if self.score < 1000:
							bullet = Bullet(self.player.rect.x + 23, self.player.rect.y +5, -1, 15, "bullet", "center")
							self.bullets.add(bullet)
							self.allSprites.add(self.bullets)
						else:
							leftbullet = Bullet(self.player.rect.x + 8, self.player.rect.y + 5, -1, 15, "bullet", "left")
							rightbullet = Bullet(self.player.rect.c + 38, self.player.rect.y + 5, -1, 15, "bullet", "right")
							self.bullets.add(leftbullet)
							self.bullets.add(rightbullet)
							self.allSprites.add(self.bullets)

	def make_enemies(self):
		enemies = sprite.Group()
		for row in range(5):
			for column in range(10):
				enemy = Enemies(row, column)
				enemy.rect.x = 157 + (column * 50)
				enemy.rect.y = 80 + (row * 45)
				enemies.add(enemy)
		
		self.enemies = enemies
		self.allSprites = sprite.Group(self.player, self.enemies, self.livesGroup, self.bossShip)


	def make_enemies_shoot(self):
		columnList = []
		for enemy in self.enemies:
			columnList.append(enemy.column)
		columnSet = set(columnList)
		columnList = list(columnSet)
		shuffle(columnList)
		column = columnList[0]
		enemyList = []
		rowList = []

		for enemy in self.enemies:
			if enemy.column == column:
				rowList.append(enemy.row)
		row = max(rowList)
		for enemy in self.enemies:
			if enemy.column == column and enemy.row == row:
				if (time.get_ticks() - self.timer) > 700:
					self.enemyBullets.add(Bullet(enemy.rect.x + 14, enemy.rect.y + 20, 1, 5, "bullet", "center"))
					self.allSprites.add(self.enemyBullets)
					self.timer = time.get_ticks()

	def calculate_score(self, row):
		scores = {0: 30,
				  1: 20,
				  2: 20,
				  3: 10,
				  4: 10,
				  5: choice([50, 100, 150, 300])
				  }
		score = scores[row]
		self.score += score
		return score

	def create_main_menu(self):
		self.enemy1 = PICTURES["enemy_C"]
		self.enemy1 = transform.scale(self.enemy1 , (40, 40))
		self.enemy2 = PICTURES["enemy_B"]
		self.enemy2 = transform.scale(self.enemy2 , (40, 40))
		self.enemy3 = PICTURES["enemy_A"]
		self.enemy3 = transform.scale(self.enemy3 , (40, 40))
		self.enemy4 = PICTURES["boss_fight"]
		self.enemy4 = transform.scale(self.enemy4 , (80, 40))
		self.screen.blit(self.enemy1, (318, 270))
		self.screen.blit(self.enemy2, (318, 320))
		self.screen.blit(self.enemy3, (318, 370))
		self.screen.blit(self.enemy4, (299, 420))

		for e in event.get():
			if e.type == QUIT:
				sys.exit()
			if e.type == KEYUP:
				self.startGame = True
				self.mainScreen = False

	def update_enemy_speed(self):
		if len(self.enemies) <= 10:
			for enemy in self.enemies:
				enemy.moveTime = 400
		if len(self.enemies) == 1:
			for enemy in self.enemies:
				enemy.moveTime = 600

	def check_collisions(self):
		collidedict = sprite.groupcollide(self.bullets, self.enemyBullets, True, False)
		if collidedict:
			for value in collidedict.values():
				for currentSprite in value:
					self.enemyBullets.remove(currentSprite)
					self.allSprites.remove(currentSprite)
		enemiesdict = sprite.groupcollide(self.bullets, self.enemies, True, False)
		if enemiesdict:
			for value in enemiesdict.values():
				for currentSprite in value:
					self.deadRow = currentSprite.row
					self.deadColumn = currentSprite.column
					score = self.calculate_score(currentSprite.row)
					explosion = Collision(currentSprite.rect.x, currentSprite.rect.y, currentSprite.row, False, False, score)
					self.explosionsGroup.add(explosion)
					self.allSprites.remove(currentSprite)
					self.enemies.remove(currentSprite)
					self.gameTimer = time.get_ticks()
					break

		mysterydict = sprite.groupcollide(self.bullets, self.bossGroup, True, True)
		if mysterydict:
			for value in mysterydict.values():
				for currentSprite in value:
					score = self.calculate_score(currentSprite.row)
					explosion = Collision(currentSprite.rect.x, currentSprite.rect.y, currentSprite.row, False, True, score)
					self.explosionsGroup.add(explosion)
					self.allSprites.remove(currentSprite)
					self.bossGroup.remove(currentSprite)
					newShip = boss_fight()
					self.allSprites.add(newShip)
					self.bossGroup.add(newShip)
					break

		bulletsdict = sprite.groupcollide(self.enemyBullets, self.playerGroup, True, False)
		if bulletsdict:
			for value in bulletsdict.values():
				for playerShip in value:
					if self.lives == 3:
						self.lives -= 1
						self.livesGroup.remove(self.life3)
						self.allSprites.remove(self.life3)
					elif self.lives == 2:
						self.lives -= 1
						self.livesGroup.remove(self.life2)
						self.allSprites.remove(self.life2)
					elif self.lives == 1:
						self.lives -= 1
						self.livesGroup.remove(self.life1)
						self.allSprites.remove(self.life1)
					elif self.lives == 0:
						self.gameOver = True
						self.startGame = False
					explosion = Collision(playerShip.rect.x, playerShip.rect.y, 0, True, False, 0)
					self.explosionsGroup.add(explosion)
					self.allSprites.remove(playerShip)
					self.playerGroup.remove(playerShip)
					self.makeNewShip = True
					self.shipTimer = time.get_ticks()
					self.shipAlive = False
		if sprite.groupcollide(self.enemies, self.playerGroup, True, True):
			self.gameOver = True
			self.startGame = False

		sprite.groupcollide(self.bullets, self.allBlockers, True, True)
		sprite.groupcollide(self.enemyBullets, self.allBlockers, True, True)
		sprite.groupcollide(self.enemies, self.allBlockers, False, True)

	def create_new_ship(self, createShip, currentTime):
		if createShip and (currentTime - self.shipTimer > 900):
			self.player = MotherShip()
			self.allSprites.add(self.player)
			self.playerGroup.add(self.player)
			self.makeNewShip = False
			self.shipAlive = True

	def create_game_over(self, currentTime):
		self.screen.blit(self.background, (0,0))
		if currentTime - self.timer < 750:
			self.gameOverText.draw(self.screen)
		if currentTime - self.timer > 750 and currentTime - self.timer < 1500:
			self.screen.blit(self.background, (0,0))
		if currentTime - self.timer > 1500 and currentTime - self.timer < 2250:
			self.gameOverText.draw(self.screen)
		if currentTime - self.timer > 2250 and currentTime - self.timer < 2750:
			self.screen.blit(self.background, (0,0))
		if currentTime - self.timer > 3000:
			self.mainScreen = True

		for e in event.get():
			if e.type == QUIT:
				sys.exit()

	def main(self):
		running = True
		while running:
			
			if self.mainScreen:
				self.reset(0,3)
				self.screen.blit(self.background, (0,0))
				self.title.draw(self.screen)
				self.enemyA.draw(self.screen)
				self.enemyB.draw(self.screen)
				self.enemyC.draw(self.screen)
				self.enemyBoss.draw(self.screen)
				self.create_main_menu()
			elif self.startGame:
				if len(self.enemies) == 0:
					currentTime = time.get_ticks()
					if currentTime - self.gameTimer < 3000:
						self.screen.blit(self.background, (0,0))
						self.scoreText2 = Text(FONTS, 20, str(self.score), GREEN, 85, 5)
						self.scoreText.draw(self.screen)
						self.scoreText2.draw(self.screen)
						self.nextRound.draw(self.screen)
						self.livesText.draw(self.screen)
						self.livesGroup.update(self.keys)
						self.userInput()
					if currentTime - self.gameTimer > 3000:
						self.reset(self.score, self.lives)
						self.enemyposition += 35
						self.make_enemies()
						self.gameTimer += 3000
				else:
					currentTime = time.get_ticks()
					self.screen.blit(self.background, (0,0))
					self.allBlockers.update(self.screen)
					self.scoreText2 = Text(FONTS, 20, str(self.score), GREEN, 85, 5)
					self.scoreText.draw(self.screen)
					self.scoreText2.draw(self.screen)
					self.livesText.draw(self.screen)
					self.userInput()
					self.allSprites.update(self.keys, currentTime, self.deadRow, self.deadColumn, self.deadArray)
					self.explosionsGroup.update(self.keys, currentTime)
					self.check_collisions()
					self.create_new_ship(self.makeNewShip, currentTime)
					self.update_enemy_speed()
					if len(self.enemies) > 0:
						self.make_enemies_shoot()

			elif self.gameOver:
				currentTime = time.get_ticks()
				self.create_game_over(currentTime)

			display.update()
			self.clock.tick(60)
		pygame.quit ()


if __name__ == '__main__':
	game = StartGame()
	game.main()

