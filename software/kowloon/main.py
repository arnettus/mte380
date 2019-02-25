import pygame, sys, robot, world
from pygame.locals import *

BROWN = (255, 221, 153)
BLUE = (0, 0, 255)
YELLOW = (204, 204, 0)
SLATE = (105,105,105)
OLIVE = (85,107,47)
MAROON = (128,0,0)
MEDPURPLE = (147,112,219)
AQUA = (126, 186, 181)

FLAT = "FLAT"
GRAV = "GRAVEL"
SAND = "SAND"
WATE = "WATER"
FOOD = "FOOD"
MIS1 = "CANDLE"
MIS2 = "GROUP"
MIS3 = "PERSON"

colours = {
    FLAT: BROWN,
    GRAV: SLATE,
    WATE: BLUE,
    SAND: YELLOW,
    FOOD: OLIVE,
    MIS1: MAROON,
    MIS2: MEDPURPLE,
    MIS3: AQUA
}

tile_map = [
    [SAND, MIS3, FLAT, SAND, MIS2, FLAT],
    [FLAT, WATE, FLAT, FLAT, FOOD, WATE],
    [FLAT, FLAT, GRAV, FLAT, FLAT, FLAT],
    [SAND, FLAT, FLAT, WATE, FLAT, GRAV],
    [MIS1, FLAT, FLAT, FLAT, SAND, FLAT],
    [FLAT, GRAV, FLAT, FLAT, GRAV, FLAT]
]

tile_size = 60
map_length = 6
map_width = 6
offset = 7

pygame.display.set_caption("Kowloon: Walled City")

pygame.init()
display_surf = pygame.display.set_mode(((map_width*2+1)*tile_size, map_length*tile_size))

#initialize player
r = robot.Robot(map_width, map_length)

# plot the world
world.build(map_length, map_width, tile_size, display_surf, colours, tile_map)
r.plot_player(display_surf, tile_size)
r.plot_map(display_surf, map_width, map_length, tile_size, offset)

# run the world
world.run()
