import pygame
import random
import math
from pygame import mixer
#zainicjowanie pygame
pygame.init()
WIDTH = 800
HEIGHT = 600

screen = pygame.display.set_mode((WIDTH , HEIGHT)) #utworzenie okienka
pygame.display.set_caption('Space Invaders')
icon = pygame.image.load('spaceship.png') #dodanie ikony okna
pygame.display.set_icon(icon)
#dodanie tła
background = pygame.image.load('space.png')
#dzwiek tła
mixer.music.load('background.wav')
mixer.music.play(-1)

playerImg = pygame.image.load('battleship.png') #dodanie ikony gracza
playerX = 370
playerY = 480
playerX_change = 0


enemyImg = []
enemyX = []
enemyY = []
enemyX_change = []
enemyY_change = []
num_of_enemies = 4

for i in range(num_of_enemies):
    enemyImg.append(pygame.image.load('ghost.png')) #dodanie ikony wroga
    enemyX.append(random.randint(0, 735))
    enemyY.append(random.randint(50, 150))
    enemyX_change.append(2)
    enemyY_change.append(25)

#'ready' -  nie można zobaczyć pocisku
#'fire' - pocisk się porusza
bulletImg = pygame.image.load('bullet.png') #dodanie pocisku
bulletX = 0
bulletY = 480
bulletX_change = 0
bulletY_change = 5
bullet_state = 'ready'

#wynik
score_value = 0
font = pygame.font.Font('freesansbold.ttf', 32)
textX = 10
textY = 10

over_font = pygame.font.Font('freesansbold.ttf', 64)

def show_score(x, y):
    score = font.render('Score: ' + str(score_value), True, (255, 255, 255))
    screen.blit(score, (x, y))

def game_over_text():
    over_text = over_font.render('GAME OVER', True, (255, 255, 255))
    screen.blit(over_text, (250, 250))

def player(x, y):
    screen.blit(playerImg, (x, y))

def enemy(x, y, i):
    screen.blit(enemyImg[i], (x, y))

def fire_bullet(x, y):
    global bullet_state
    bullet_state = 'fire'
    screen.blit(bulletImg, (x+16, y+10))

def isCollision(enemyX, enemyY, bulletX, bulletY):
    distance = math.sqrt((math.pow(enemyX-bulletX,2)) + (math.pow(enemyY-bulletY,2)))
    if distance < 27:
        return True
    else:
        return False

#pętla dzięki której gra się nie zamyka
running = True
while running:
    screen.fill((30, 30, 30))

    screen.blit(background, (0, 0))

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_a:
                playerX_change = -4
            if event.key == pygame.K_d:
                playerX_change = 4
            if event.key == pygame.K_SPACE:
                if bullet_state is 'ready':
                    bullet_sound = mixer.Sound('laser.wav')
                    bullet_sound.play()
                    #dostajemy obecne polozenie statku (x)
                    bulletX = playerX
                    fire_bullet(bulletX, bulletY)

        if event.type == pygame.KEYUP:
            if event.key == pygame.K_a or event.key == pygame.K_d:
                playerX_change = 0

#limit ruchów
    playerX += playerX_change

    if playerX <= 0:
        playerX = 0
    elif playerX >= 736:
        playerX = 736
#ruch przeciwnika
    for i in range(num_of_enemies):

        #koniec gry
        if enemyY[i] > 440:
            for j in range(num_of_enemies):
                enemyY[j] = 2000
            game_over_text()
            break

        enemyX[i] += enemyX_change[i]
        if enemyX[i] <= 0:
            enemyX_change[i] = 2
            enemyY[i] += enemyY_change[i]
        elif enemyX[i] >= 736:
            enemyX_change[i] = -2
            enemyY[i] += enemyY_change[i]

        #zderzenie
        collision = isCollision(enemyX[i], enemyY[i], bulletX, bulletY)
        if collision:
            bulletY = 480
            bullet_state = 'ready'
            score_value += 1
            enemyX[i] = random.randint(0, 735)
            enemyY[i] = random.randint(50, 150)

        enemy(enemyX[i], enemyY[i], i)

    #ruch pocisku
    if bulletY <= 0:
        bulletY = 480
        bullet_state = 'ready'
    if bullet_state == 'fire':
        fire_bullet(bulletX, bulletY)
        bulletY -= bulletY_change




    player(playerX, playerY)
    show_score(textX, textY)
    pygame.display.update()

