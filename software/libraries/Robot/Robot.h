#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>

const int MAP_WIDTH = 6;
const int MAP_HEIGHT = 6;

enum State {
    PATH_PLAN_SURVEY_A,
    PATH_PLAN_SURVEY_B,
    PATH_PLAN,
    HOUSE,
    STRAIGHT,
    TURN_RIGHT,
    TURN_LEFT,
};

enum Tile {
    UNKNOWN,
    FLAT,
    WATER,
    SAND,
    MISSION,
    GRAVEL,
};


class Robot {
private:
  Tile grid[MAP_WIDTH][MAP_HEIGHT];

  void InitializeGrid() {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
      for (int j = 0; j < MAP_HEIGHT; ++j) {
        grid[i][j] = UNKNOWN;
      }
    }
  }

public:
  Robot(){
    InitializeGrid();
  }

  Tile ReadGrid(int x, int y){
    return grid[y][x];
  }

  void WriteGrid(int x, int y, Tile t){
    grid[y][x] = t;
  }
};

#endif
