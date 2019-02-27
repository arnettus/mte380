from pq import PriorityQueue

def heuristic(a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)

def a_star(grid, start, goal):
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

        for nxt in neighbors(grid, current):
            new_cost = cost_so_far[current] + grid.cost(current, nxt)
            if n not in cost_so_far or new_cost < cost_so_far[nxt]:
                cost_so_far[nxt] = new_cost
                priority = new_cost + heuristic(goal, nxt)
                frontier.put(nxt, priority)
                came_from[nxt] = current

    return came_from, cost_so_far

def neighbours(grid, pos):
    n = []
    top_boundary = 0
    left_boundary = 0
    bottom_boundary = len(grid) - 1
    right_boundary = len(grid[0]) - 1

    if pos[1] > top_boundary: # above
        n.append([pos[0], pos[1]-1])
    if pos[1] < bottom_boundary: # below
        n.append([pos[0], pos[1]+1])
    if pos[0] > left_boundary: #left
        n.append([pos[0]-1, pos[1]])
    if pos[0] < right_boundary: # right
        n.append([pos[0]+1, pos[1]])

    return n
