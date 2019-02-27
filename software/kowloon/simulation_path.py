from pq import PriorityQueue

def a_star_path(grid, start, goal):
    came_from = a_star_search(grid, start, goal)
    return reconstruct_path(came_from, start, goal)

def a_star_search(grid, start, goal):
    frontier = PriorityQueue()
    frontier.put(start, 0)

    came_from = {}
    cost_so_far = {}

    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
        current = frontier.get()

        if current == goal:
            break

        for nxt in neighbours(grid, current):
            new_cost = cost_so_far[current] + cost(grid, current, nxt)

            if nxt not in cost_so_far or new_cost < cost_so_far[nxt]:
                cost_so_far[nxt] = new_cost
                priority = new_cost + heuristic(goal, nxt)

                # try to update test grid to see it move
                show_frontier(nxt)
                # ----

                frontier.put(nxt, priority)
                came_from[nxt] = current

    return came_from


# make a third display to show it thinking??????
# nah just do what you said you'd do in the comments on PR
def show_frontier(coordinate):
    pass

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

    # optional, probably don't want this, if path is a stack,
    # then just pop in O(1)

    return path

def neighbours(grid, pos):
    n = []

    top_boundary = 0
    left_boundary = 0
    bottom_boundary = len(grid) - 1
    right_boundary = len(grid[0]) - 1

    # use a lambda with a filter here instead possibly??

    if pos[1] > top_boundary: # above
        n.append((pos[0], pos[1]-1))
    if pos[1] < bottom_boundary: # below
        n.append((pos[0], pos[1]+1))
    if pos[0] > left_boundary: #left
        n.append((pos[0]-1, pos[1]))
    if pos[0] < right_boundary: # right
        n.append((pos[0]+1, pos[1]))

    return n

# gr = [[0 for i in range(6)] for i in range(6)]
# start = (3, 5)
# end = (0, 0)

# p = a_star_path(gr, start, end)
# print(p)

# import pandas
# pa = pandas.DataFrame(gr)
# print(pa)
