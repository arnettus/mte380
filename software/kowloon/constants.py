import pygame
from pygame.locals import *

tile_size = 60
map_length = 6
map_width = 6
offset = 7

pygame.display.set_caption("Kowloon: Walled City")

pygame.init()
display_surf = pygame.display.set_mode(((map_width*2+1)*tile_size, map_length*tile_size))

blank = pygame.image.load('images/blank.png').convert_alpha()

FLAT = "FLAT"
GRAV = "GRAVEL"
SAND = "SAND"
WATE = "WATER"
FOOD = "FOOD"
MIS1 = "CANDLE"
MIS2 = "GROUP"
MIS3 = "PERSON"

colours = {
    FLAT: pygame.image.load('images/flat.png').convert_alpha(),
    GRAV: pygame.image.load('images/gravel.png').convert_alpha(),
    WATE: pygame.image.load('images/water.png').convert_alpha(),
    SAND: pygame.image.load('images/sand.png').convert_alpha(),
    FOOD: pygame.image.load('images/food.png').convert_alpha(),
    MIS1: pygame.image.load('images/fire.png').convert_alpha(),
    MIS2: pygame.image.load('images/group.png').convert_alpha(),
    MIS3: pygame.image.load('images/person.png').convert_alpha()
}

tile_map = [
    [SAND, MIS3, FLAT, SAND, MIS2, FLAT],
    [FLAT, WATE, FLAT, FLAT, FOOD, WATE],
    [FLAT, FLAT, GRAV, FLAT, FLAT, FLAT],
    [SAND, FLAT, FLAT, WATE, FLAT, GRAV],
    [MIS1, FLAT, FLAT, FLAT, SAND, FLAT],
    [FLAT, GRAV, FLAT, FLAT, GRAV, FLAT]
]
