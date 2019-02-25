import pygame, sys
from pygame.locals import *

def build(map_length, map_width, tile_size, display_surf, colours, tile_map):
    for row in range(map_length):
        for col in range(map_width):
            pygame.draw.rect(
                display_surf,
                colours[tile_map[row][col]],
                (col*tile_size, row*tile_size, tile_size, tile_size)
            )

def run():
    while True:
        for event in pygame.event.get():
            # Prompt user to click on the boxes for each part
            # of the map. Then update display_surf
            if event.type == QUIT:
                pygame.quit()
                sys.exit()

        pygame.display.update()