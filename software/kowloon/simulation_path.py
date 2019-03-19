from pq import PriorityQueue

class Node(object):
    def __init__(self, x = -100, y = -100, cost = 0, parent = None):
        self.parent = parent
        self.cost = cost
        self.seen = False

def a_star_path(start, goal):
    return a_star_search(start, goal)

def a_star_search(start, goal):
    aux_grid = [[Node() for i in range(6)] for i in range(6)]

    frontier = PriorityQueue()
    frontier.put(start, 0)

    start_x = start[0]
    start_y = start[1]

    aux_grid[start_x][start_y].seen = True

    while not frontier.empty():
        current = frontier.get()

        if current == goal: # use coordinates
            break

        for nxt in neighbours(aux_grid, current): # returns a list of valid neighbours
            nxt_x = nxt[0]
            nxt_y = nxt[1]
            curr_x = current[0]
            curr_y = current[1]

            new_cost = aux_grid[curr_x][curr_y].cost + cost(aux_grid, current, nxt) # put turn and tile cost here

            if aux_grid[nxt_x][nxt_y].seen == False or new_cost < aux_grid[nxt_x][nxt_y].cost:
                aux_grid[nxt_x][nxt_y].cost = new_cost

                priority = new_cost + heuristic(goal, nxt) # keep at manhattan distance

                frontier.put(nxt, priority)
                aux_grid[nxt_x][nxt_y].parent = current
                aux_grid[nxt_x][nxt_y].seen = True

    return aux_grid

def heuristic(a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)

# this is the weight of the edge???
def cost(self, from_node, to_node):
    # adjust this based on tile costttt
    return 1

def reconstruct_path(came_from, start, goal):
    current = goal
    path = []

    while current != start:
        path.append(current)
        current = came_from[current]

    return path

def neighbours(grid, pos):
    n = []

    top_boundary = 0
    left_boundary = 0
    bottom_boundary = len(grid) - 1
    right_boundary = len(grid[0]) - 1

    if pos[1] > top_boundary: # above
        n.append((pos[0], pos[1]-1))
    if pos[1] < bottom_boundary: # below
        n.append((pos[0], pos[1]+1))
    if pos[0] > left_boundary: #left
        n.append((pos[0]-1, pos[1]))
    if pos[0] < right_boundary: # right
        n.append((pos[0]+1, pos[1]))

    return n


start = (3, 5)
end = (0, 0)

g = a_star_path(start, end)
print(g[0][0].parent)


gr = [[0 for i in range(6)] for i in range(6)]

for x in range(6):
    for y in range(6):
        gr[x][y] = g[x][y].cost

import pandas
print(pandas.DataFrame(gr))
