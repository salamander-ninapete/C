#include "tetris.h"

bool moveFigureDown(GameInfo_t *game, TetFigure *figure) {
  bool val = true;
  if (checkCollision(*game, *figure, 0, 1)) {
    washTraceFigure(game, figure);
    figure->y++;
    drawFigure(game, figure);
  } else {
    val = stopFigure(game, figure, val);
  }
  return val;
}

bool stopFigure(GameInfo_t *game, TetFigure *figure, bool val) {
  placeFigureOnField(game, figure);
  *figure = placeFigure(game);
  if (!checkCollision(*game, *figure, 0, 1)) val = false;
  return val;
}

TetFigure *moveRight(GameInfo_t *game, TetFigure *figure) {
  if (checkCollision(*game, *figure, 1, 0)) {
    washTraceFigure(game, figure);
    figure->x += 1;
  }
  drawFigure(game, figure);
  return figure;
}

TetFigure *moveLeft(GameInfo_t *game, TetFigure *figure) {
  if (checkCollision(*game, *figure, -1, 0)) {
    washTraceFigure(game, figure);
    figure->x -= 1;
  }
  drawFigure(game, figure);
  return figure;
}

TetFigure *handleRotation(GameInfo_t *game, TetFigure *figure) {
  if (canRotate(*game, *figure)) {
    washTraceFigure(game, figure);
    figure->currentState = (figure->currentState + 1) % 4;
  }
  drawFigure(game, figure);
  return figure;
}

bool canRotate(GameInfo_t game, TetFigure figure) {
  washTraceFigure(&game, &figure);
  int nextState = (figure.currentState + 1) % FIGURE_STATE;
  TetFigure rotated = figure;
  rotated.currentState = nextState;
  bool accept = true;
  for (int i = 0; i < rotated.size; i++) {
    for (int j = 0; j < rotated.size; j++) {
      if (rotated.blocks[nextState][i][j] != 0) {
        int newX = rotated.x + j + (WIDTH / 2) - 2;
        int newY = rotated.y + i + SDWIG;
        if (newX < 0 || newX >= WIDTH || newY >= HEIGHT + SDWIG) {
          accept = false;
        }
        if (newY >= 0 && game.field[newY][newX] != 0) {
          accept = false;
        }
      }
    }
  }
  return accept;
}

bool checkCollision(GameInfo_t game, TetFigure figure, int dx, int dy) {
  washTraceFigure(&game, &figure);
  bool ans = true;
  for (int i = 0; i < figure.size; i++) {
    for (int j = 0; j < figure.size; j++) {
      if (figure.blocks[figure.currentState][i][j] != 0) {
        int newX = figure.x + j + dx + (WIDTH / 2) - 2;
        int newY = figure.y + i + SDWIG + dy;
        if (newX < 0 || newX >= WIDTH || newY >= HEIGHT + SDWIG) {
          ans = false;
        } else if (newY >= 0 && game.field[newY][newX] != 0) {
          ans = false;
        }
      }
    }
  }

  return ans;
}
