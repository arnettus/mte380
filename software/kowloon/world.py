import pygame, sys, time

from constants import *
from pygame.locals import *

def build():
    for row in range(map_length):
        for col in range(map_width):
            display_surf.blit(
                colours[tile_map[row][col]],
                (col*tile_size, row*tile_size)
            )

def travel(robot, old_coor, coordinate):
    display_surf.blit(
        colours[tile_map[old_coor[1]][old_coor[0]]],
        (old_coor[0]*tile_size, old_coor[1]*tile_size)
    )

    robot.pos = coordinate

    display_surf.blit(
        robot.player,
        ((robot.pos[0])*tile_size, (robot.pos[1])*tile_size)
    )

def run(robot):
    fpsClock = pygame.time.Clock()
    pygame.display.update()
    old_pos = robot.pos
    new_pos = (2, 0)
    p = robot.plan_path(old_pos, new_pos)

    print(p)
    while True:
        if len(p):
            coordinate = p.pop()
            robot.travel(robot.pos, coordinate)

        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()

        pygame.time.wait(500)
        pygame.display.update()
        # add pygame.clock right here
