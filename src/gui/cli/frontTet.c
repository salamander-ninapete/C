#include "../../brick_game/tetris/tetris.h"

void renderGame(GameInfo_t *game) {
  clear();
  for (int y = 1; y < HEIGHT + SDWIG; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game->field[y][x]) {
        mvprintw(y, x * 2, "[]");
      } else {
        mvprintw(y, x * 2, ". ");
      }
    }
  }
  mvprintw(1, WIDTH * 2 + 2, "Score: %d", game->score);
  mvprintw(2, WIDTH * 2 + 2, "High Score: %d", game->high_score);
  mvprintw(3, WIDTH * 2 + 2, "Level: %d", game->level);
  renderNextFigure(game);
  if (game->pause) {
    mvprintw(COORDINATE_NEXT_FIG + 4, WIDTH * 2 + 2, "PAUSED");
  }
  refresh();
}

void renderNextFigure(GameInfo_t *game) {
  if (game->next_fig == 0) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "[][][][]");
  }
  if (game->next_fig == 1) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "[][]");
    mvprintw(COORDINATE_NEXT_FIG + one_row, WIDTH * 2 + 2, "[][]");
  }
  if (game->next_fig == 2) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "  []");
    mvprintw(COORDINATE_NEXT_FIG + one_row, WIDTH * 2 + 2, "[][]");
    mvprintw(COORDINATE_NEXT_FIG + two_row, WIDTH * 2 + 2, "  []");
  }
  if (game->next_fig == 3) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "[]");
    mvprintw(COORDINATE_NEXT_FIG + one_row, WIDTH * 2 + 2, "[][]");
    mvprintw(COORDINATE_NEXT_FIG + two_row, WIDTH * 2 + 2, "  []");
  }
  if (game->next_fig == 4) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "  []");
    mvprintw(COORDINATE_NEXT_FIG + one_row, WIDTH * 2 + 2, "[][]");
    mvprintw(COORDINATE_NEXT_FIG + two_row, WIDTH * 2 + 2, "[]");
  }
  if (game->next_fig == 5) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "[][]");
    mvprintw(COORDINATE_NEXT_FIG + one_row, WIDTH * 2 + 2, "[]");
    mvprintw(COORDINATE_NEXT_FIG + two_row, WIDTH * 2 + 2, "[]");
  }
  if (game->next_fig == 6) {
    mvprintw(COORDINATE_NEXT_FIG + zer_row, WIDTH * 2 + 2, "[][]");
    mvprintw(COORDINATE_NEXT_FIG + one_row, WIDTH * 2 + 2, "  []");
    mvprintw(COORDINATE_NEXT_FIG + two_row, WIDTH * 2 + 2, "  []");
  }
}

void showMenu() {
  clear();
  mvprintw(10, 11, "START GAME (s)");
  mvprintw(11, 12, "QUIT GAME (q)");
  refresh();
}

void showGameOver(GameInfo_t *game) {
  clear();
  mvprintw(10, 11, "GAME OVER");
  mvprintw(11, 11, "YOUR SCORE: %d", game->score);
  refresh();
}

void showGameOver2() {
  clear();
  mvprintw(10, 11, "SEE YOU LATER");
  refresh();
}
