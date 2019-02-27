import pandas, pygame
import simulation_path
import time

from constants import *

UNKNOWN = "U"

class Robot(object):
    def __init__(self):
        self.player = pygame.image.load('images/mudkip.png').convert_alpha()
        self.pos = (map_width // 2, map_length - 1)
        self.map = build_map()

    def plot_map(self):
        for row in range(map_length):
            for col in range(map_width):
                x = offset+row
                y = col

                display_surf.blit(blank,(x*tile_size, y*tile_size))

        display_surf.blit(
            self.player,
            ((self.pos[0]+offset)*tile_size, self.pos[1]*tile_size)
        )

    def plot_player(self):
        display_surf.blit(
            self.player,
            (self.pos[0]*tile_size, self.pos[1]*tile_size)
        )

    def print_grid(self):
        pass


    def plan_path(self, old, coordinate):
        # this will have to be a while true loop to stop when the
        # robot sees water or when it detects sand, then do other stuff
        # and path plan again. for now just do the thinggg

        path = simulation_path.a_star_path(self.map, old, coordinate)

        return path

    def animate_travel(self, path):

        while len(path):
            coordinate = path.pop()
            self.travel(self.pos, coordinate)


    def travel(self, old, new):
        r = display_surf.blit(
            colours[tile_map[old[1]][old[0]]],
            ((old[0]+offset)*tile_size, (old[1])*tile_size)
        )

        self.pos = new

        r = display_surf.blit(
            self.player,
            ((self.pos[0]+offset)*tile_size, (self.pos[1])*tile_size)
        )

    # use this duringggg path plannnn, and travellll
    def detect_tile(self, display_surf, coordinate):
        pass

def build_map():
    return [[UNKNOWN for i in range(map_width)] for i in range(map_length)]

def bad_delay(s):
    t_end = time.time() + s

    i = 0
    while time.time() < t_end:
        i += 1
