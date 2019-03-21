from pq import PriorityQueue

class Node(object):
    def __init__(self, x = -100, y = -100, cost = 0, parent = None):
        self.parent = parent
        self.cost = cost
        self.seen = False
        self.dir = "NORTH"

def a_star_search(start, goal):
    aux_grid = [[Node() for i in range(6)] for i in range(6)]

    frontier = []
    frontier.append(start)

    start_x = start[1]
    start_y = start[0]

    aux_grid[start_x][start_y].seen = True
    initialOrientation = "NORTH"
    initialCheck = True

    while len(frontier):
        current = frontier.pop(0)

        if current == goal: # use coordinates
            break

        for nxt in neighbours(aux_grid, current): # returns a list of valid neighbours
            nxt_x = nxt[1]
            nxt_y = nxt[0]
            curr_x = current[1]
            curr_y = current[0]

            new_cost = aux_grid[curr_x][curr_y].cost + cost(initialCheck, initialOrientation, current, aux_grid[curr_x][curr_y].parent, nxt)

            if aux_grid[nxt_x][nxt_y].seen == False or new_cost < aux_grid[nxt_x][nxt_y].cost:
                aux_grid[nxt_x][nxt_y].cost = new_cost

                priority = new_cost + heuristic(goal, nxt) # keep at manhattan distance

                frontier.append(nxt)
                aux_grid[nxt_x][nxt_y].parent = current
                aux_grid[nxt_x][nxt_y].seen = True

    return aux_grid

def heuristic(a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)

# this is the weight of the edge???
def cost(initialCheck, initialOrientation, from_node, from_node_parent, to_node):
    return travel_cost() + turn_cost(initialCheck, initialOrientation, from_node, from_node_parent, to_node)

def travel_cost():
    return 1

def turn_cost(initialCheck, initialOrientation, from_node, from_node_parent, to_node):
    if initialCheck:
        initialCheck = False
        orientation = initialOrientation
    else:
        if from_node_parent[0] < from_node[0]:
            orientation = "NORTH"
        elif from_node_parent[0] > from_node[0]:
            orientation = "SOUTH"
        elif from_node_parent[1] < from_node[1]:
            orientation = "WEST"
        else:
            orientation = "EAST"

    if orientation == "NORTH":
        if to_node[0] < from_node[0]:
            return 0
    elif orientation == "SOUTH":
        if to_node[0] > from_node[0]:
            return 0
    elif orientation == "WEST":
        if to_node[1] < from_node[1]:
            return 0
    else: # EAST
        if to_node[1] > from_node[1]:
            return 0

    return 1

def neighbours(grid, pos):
    n = []

    top_boundary = 0
    left_boundary = 0
    bottom_boundary = len(grid) - 1
    right_boundary = len(grid[0]) - 1

    if pos[1] > top_boundary: # above
        if main_grid[pos[1]-1][pos[0]] == "":
            n.append((pos[0], pos[1]-1))
    if pos[1] < bottom_boundary: # below
        if main_grid[pos[1]+1][pos[0]] == "":
            n.append((pos[0], pos[1]+1))
    if pos[0] > left_boundary: #left
        if main_grid[pos[1]][pos[0]-1] == "":
            n.append((pos[0]-1, pos[1]))
    if pos[0] < right_boundary: # right
        if main_grid[pos[1]][pos[0]+1] == "":
            n.append((pos[0]+1, pos[1]))

    return n

def reconstruct_path(grid, end):
    current = end
    currDir = "NONE"
    path = []

    # while grid[current[1]][current[0]].parent != None:
    #     par = grid[current[1]][current[0]].parent
    #     path.append(par)
    #     current = par

    while grid[current[1]][current[0]].parent != None:
        par = grid[current[1]][current[0]].parent

        if par[1] < current[1]:
            nextDir = "WEST"
        elif par[1] > current[1]:
            nextDir = "EAST"
        elif par[0] < current[0]:
            nextDir = "NORTH"
        else:
            nextDir = "SOUTH"

        if currDir != nextDir:
            path.append(current)

        current = par

        currDir = nextDir

    return path

start = (3, 0)
end = (0, 5)

main_grid = [
    ["", "", "", "", "", ""],
    ["", "", "", "", "", ""],
    ["", "", "", "", "", ""],
    ["", "", "", "", "", ""],
    ["", "", "", "", "", ""],
    ["", "", "", "", "", ""]
]

cost_grid = a_star_search(start, end)

print(reconstruct_path(cost_grid, end))

gr = [[0 for i in range(6)] for i in range(6)]

for x in range(6):
    for y in range(6):
        gr[x][y] = cost_grid[x][y].cost

import pandas
print(pandas.DataFrame(gr))
