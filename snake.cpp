#include <iostream>
#include <raylib.h>

#define WINDOW_WIDTH 646
#define WINDOW_HEIGHT 480

#define GRID_WIDTH 17
#define GRID_HEIGHT 15

enum Direction { VOID, RIGHT, UP, LEFT, DOWN };

struct Snake {
  int x = 0;
  int y = 0;
  int size = 1;
  Direction dir = DOWN;
};

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");

  int grid[GRID_WIDTH][GRID_HEIGHT] = {0};
  bool apples[GRID_WIDTH][GRID_HEIGHT] = {false};
  bool hasApple = false;

  const int RATIO_WIDTH = WINDOW_WIDTH / GRID_WIDTH;
  const int RATIO_HEIGHT = WINDOW_HEIGHT / GRID_HEIGHT;

  Snake snake;

  Direction strategy[GRID_WIDTH][GRID_HEIGHT] = {VOID};
  strategy[0][0] = DOWN;
  strategy[GRID_WIDTH - 1][0] = LEFT;
  strategy[0][1] = DOWN;
  strategy[1][1] = LEFT;
  for (int i = 2; i < GRID_WIDTH - 1; i += 2) {
    strategy[i][1] = RIGHT;
    strategy[i + 1][1] = DOWN;
  }
  strategy[1][2] = RIGHT;
  strategy[2][2] = UP;
  for (int i = 3; i < GRID_HEIGHT; i += 2) {
    strategy[1][i] = UP;
    strategy[2][i] = LEFT;
    strategy[1][i + 1] = RIGHT;
    strategy[2][i + 1] = UP;
  }
  strategy[0][GRID_HEIGHT - 1] = RIGHT;
  for (int i = 3; i < GRID_WIDTH; i += 2) {
    strategy[i][GRID_HEIGHT - 1] = RIGHT;
    strategy[i + 1][GRID_HEIGHT - 1] = UP;
  }

  bool win = false;

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (snake.x == 1 && snake.y == 0) {
      if (apples[0][0]) {
        strategy[1][0] = LEFT;
      } else {
        strategy[1][0] = DOWN;
      }
    }

    switch (GetKeyPressed()) {
    case KEY_RIGHT:
      if (snake.dir != LEFT)
        snake.dir = RIGHT;
      break;
    case KEY_UP:
      if (snake.dir != DOWN)
        snake.dir = UP;
      break;
    case KEY_LEFT:
      if (snake.dir != RIGHT)
        snake.dir = LEFT;
      break;
    case KEY_DOWN:
      if (snake.dir != UP)
        snake.dir = DOWN;
      break;
    }

    switch (strategy[snake.x][snake.y]) {
    case RIGHT:
      if (snake.dir != LEFT)
        snake.dir = RIGHT;
      break;
    case UP:
      if (snake.dir != DOWN)
        snake.dir = UP;
      break;
    case LEFT:
      if (snake.dir != RIGHT)
        snake.dir = LEFT;
      break;
    case DOWN:
      if (snake.dir != UP)
        snake.dir = DOWN;
      break;
    }

    switch (snake.dir) {
    case RIGHT:
      snake.x++;
      break;
    case UP:
      snake.y--;
      break;
    case LEFT:
      snake.x--;
      break;
    case DOWN:
      snake.y++;
      break;
    }

    bool shouldDec = !apples[snake.x][snake.y];
    if (!shouldDec) {
      apples[snake.x][snake.y] = false;
      hasApple = false;
      snake.size++;
    }

    if (shouldDec)
      for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
          grid[i][j] -= grid[i][j] ? 1 : 0;
        }
      }

    if (grid[snake.x][snake.y] != 0 || snake.x > GRID_WIDTH || snake.x < 0 ||
        snake.y > GRID_HEIGHT || snake.y < 0) {
      break;
    }

    if (snake.size == GRID_HEIGHT * GRID_WIDTH) {
      win = true;
      break;
    }

    grid[snake.x][snake.y] = snake.size;

    while (!hasApple) {
      int x = rand() % GRID_WIDTH;
      int y = rand() % GRID_HEIGHT;

      if (grid[x][y])
        continue;

      apples[x][y] = true;
      hasApple = true;
    }

    BeginDrawing();
    for (int i = 0; i < GRID_WIDTH; i++) {
      for (int j = 0; j < GRID_HEIGHT; j++) {
        const auto tone = grid[i][j] ? (unsigned char)255 : (unsigned char)0;
        auto color = (Color){tone, tone, tone, 255};
        if (apples[i][j]) {
          DrawRectangle(i * RATIO_WIDTH, j * RATIO_HEIGHT, RATIO_WIDTH,
                        RATIO_HEIGHT, RED);
        } else {
          DrawRectangle(i * RATIO_WIDTH, j * RATIO_HEIGHT, RATIO_WIDTH,
                        RATIO_HEIGHT, color);
        }
      }
    }
    EndDrawing();
  }

  if (win) {
    TraceLog(LOG_INFO, "You win!");
  } else {
    TraceLog(LOG_INFO, "You lose...");
  }
}
