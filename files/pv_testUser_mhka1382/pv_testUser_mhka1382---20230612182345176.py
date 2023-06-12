import pygame
import time
import random

# Initialize Pygame
pygame.init()

# Set the game window dimensions
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

# Set the colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

# Set the font
FONT_SIZE = 32
FONT = pygame.font.SysFont(None, FONT_SIZE)

# Set the game clock
clock = pygame.time.Clock()

# Define the game functions
def print_text(message, color, x, y):
    text = FONT.render(message, True, color)
    text_rect = text.get_rect()
    text_rect.center = (x, y)
    screen.blit(text, text_rect)

def intro():
    print_text("You find yourself standing in the middle of a dark forest.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
    print_text("You can see a path to the north and a path to the east.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE)

def north_path():
    print_text("You follow the path to the north.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
    print_text("You come across a river.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE)
    print_text("You can swim across or try to find a bridge.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*2)
    pygame.display.update()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_1:
                    print_text("You start swimming across the river.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*3)
                    pygame.display.update()
                    time.sleep(2)
                    print_text("You get halfway across and realize you can't make it.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*4)
                    print_text("You turn back and head to the starting point.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*5)
                    pygame.display.update()
                    time.sleep(2)
                    play_game()
                elif event.key == pygame.K_2:
                    print_text("You find a bridge and cross the river safely.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*3)
                    print_text("You continue along the path.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*4)
                    print_text("You come across a cave.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*5)
                    pygame.display.update()
                    time.sleep(2)
                    while True:
                        for event in pygame.event.get():
                            if event.type == pygame.QUIT:
                                pygame.quit()
                                quit()
                            elif event.type == pygame.KEYDOWN:
                                if event.key == pygame.K_1:
                                    print_text("You enter the cave and find a treasure!", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*6)
                                    print_text("You win!", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*7)
                                    pygame.display.update()
                                    time.sleep(2)
                                    pygame.quit()
                                    quit()
                                elif event.key == pygame.K_2:
                                    print_text("You continue along the path and get lost.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*6)
                                    print_text("You turn back and head to the starting point.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*7)
                                    pygame.display.update()
                                    time.sleep(2)
                                    play_game()

def east_path():
    print_text("You follow the path to the east.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2)
    print_text("You come across a clearing.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE)
    print_text("You can see a castle in the distance.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*2)
    pygame.display.update()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_1:
                    print_text("You head towards the castle.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*3)
                    print_text("You come across a dragon!", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*4)
                    print_text("You can fight the dragon or tryto sneak past.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*5)
                    pygame.display.update()
                    time.sleep(2)
                    while True:
                        for event in pygame.event.get():
                            if event.type == pygame.QUIT:
                                pygame.quit()
                                quit()
                            elif event.type == pygame.KEYDOWN:
                                if event.key == pygame.K_1:
                                    print_text("You fight the dragon and lose.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*6)
                                    print_text("You turn back and head to the starting point.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*7)
                                    pygame.display.update()
                                    time.sleep(2)
                                    play_game()
                                elif event.key == pygame.K_2:
                                    print_text("You sneak past the dragon and enter the castle.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*6)
                                    print_text("You find a treasure!", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*7)
                                    print_text("You win!", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*8)
                                    pygame.display.update()
                                    time.sleep(2)
                                    pygame.quit()
                                    quit()
                elif event.key == pygame.K_2:
                    print_text("You stay in the clearing and get lost.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*3)
                    print_text("You turn back and head to the starting point.", WHITE, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + FONT_SIZE*4)
                    pygame.display.update()
                    time.sleep(2)
                    play_game()

def play_game():
    screen.fill(BLACK)
    intro()
    pygame.display.update()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_1:
                    screen.fill(BLACK)
                    north_path()
                    pygame.display.update()
                elif event.key == pygame.K_2:
                    screen.fill(BLACK)
                    east_path()
                    pygame.display.update()

# Set up the game window
screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Adventure Game")

# Start the game
play_game()

# Quit Pygame
pygame.quit()