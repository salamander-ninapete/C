#include "../../brick_game/tetris/tetris.h"

int main() {
  GameInfo_t game;
  initializeGame(&game, "highscore.txt");
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(game.speed);
  bool running = true;
  showMenu();
  running = handleMenuInput(&game);
  TetFigure figure = placeFigure(&game);
  while (running) {
    moveFigure(&running, &figure, &game);
    timeout(game.speed);
    game = *leveling(&game);
    if (game.score > game.high_score) {
      game.high_score = game.score;
      writeHighScore("highscore.txt", game.high_score);
    }
    renderGame(&game);
  }
  if (game.score == QUIT_IN_MENU)
    showGameOver2();
  else
    showGameOver(&game);
  gameOverCase();
  endwin();
  cleanupGame(&game);
  return 0;
}

bool handleMenuInput(GameInfo_t *game) {
  int ans = INF;
  while (ans == INF) {
    int ch = getch();
    switch (ch) {
      case 's':
        ans = true;
        break;
      case 'q':
        ans = false;
        game->score = QUIT_IN_MENU;
        break;
    }
  }
  return ans;
}

int gameOverCase() {  //
  int ans = INF;
  while (ans == INF) {
    int ch = getch();
    switch (ch) {
      case 'q':
        ans = true;
        break;
    }
  }
  return ans;
}

int moveFigure(bool *running, TetFigure *figure, GameInfo_t *game) {
  UserAction_t action = 0;
  int ch = getch();
  if (*running) {
    switch (ch) {
      case 'q':
        *running = false;
        action = Terminate;
        *running = userInput(action, figure, game);
        break;
      case KEY_LEFT:
        action = Left;
        *running = userInput(action, figure, game);
        break;
      case KEY_RIGHT:
        action = Right;
        *running = userInput(action, figure, game);
        break;
      case KEY_DOWN:
        action = Down;
        *running = userInput(action, figure, game);
        break;
      case ' ':
        action = Action;
        *running = userInput(action, figure, game);
        break;
      case 'p':
        action = Pause;
        *running = userInput(action, figure, game);
        break;
      case 's':
        action = Start;
        *running = userInput(action, figure, game);
        break;
      default:
        action = Up;
        *running = userInput(action, figure, game);
        break;
    }
  }
  return *running;
}
