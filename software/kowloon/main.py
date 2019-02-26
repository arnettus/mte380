import pygame, sys, robot, world

from constants import *
from pygame.locals import *

#initialize player
r = robot.Robot()

# plot the world
world.build()

r.plot_player()
r.plot_map()

# run the world
world.run(r)
