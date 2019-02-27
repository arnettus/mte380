import pandas, pygame

from constants import *

UNKNOWN = "U"

class Robot(object):
    def __init__(self):
        self.player = pygame.image.load('images/mudkip.png').convert_alpha()
        self.pos = [map_width // 2, map_length - 1]
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

    def check_square(self):
        # check square in front of you, along the path that you get from astar
        pass

    def travel(self, old, coordinate):
        # use blank to blit for nowwww but replace with the
        # coordinate that you saw or whatever, like do some detection
        display_surf.blit(
            colours[tile_map[old[1]][old[0]]],
            ((old[0]+offset)*tile_size, (old[1])*tile_size)
        )

        self.pos = coordinate

        display_surf.blit(
            self.player,
            ((self.pos[0]+offset)*tile_size, (self.pos[1])*tile_size)
        )

    def detect_tile(self, display_surf, coordinate):
        pass

def build_map():
    return [[UNKNOWN for i in range(map_width)] for i in range(map_length)]

def heuristic():
    pass
