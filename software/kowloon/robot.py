import pandas, pygame

UNKNOWN = "U"

class Robot(object):
    def __init__(self, map_width, map_length):
        self.player = pygame.image.load('mudkip.png').convert_alpha()
        self.blank = pygame.image.load('blank.png').convert_alpha()
        self.pos = [map_width // 2, map_length - 1]
        self.map = build_map(map_width, map_length)

    def plot_map(self, display_surf, map_width, map_length, tile_size, offset):
        for row in range(map_length):
            for col in range(map_width):
                x = offset+row
                y = col

                display_surf.blit(self.blank,(x*tile_size, y*tile_size))

        display_surf.blit(
            self.player,
            ((self.pos[0]+offset)*tile_size, self.pos[1]*tile_size)
        )

    def plot_player(self, display_surf, tile_size):
        display_surf.blit(
            self.player,
            (self.pos[0]*tile_size, self.pos[1]*tile_size)
        )

    def print_grid(self):
        pass

    def check_square(self):
        # check square in front of you, along the path that you get from astar
        pass

    def aStar(self, goal):
        # return a list of coordiantes
        pass

    def travel(self, display_surf, coordinate):
        pass

def build_map(map_width, map_length):
    return [[UNKNOWN for i in range(map_width)] for i in range(map_length)]

def heuristic():
    pass