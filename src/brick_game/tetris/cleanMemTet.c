#include "tetris.h"

void cleanupGame(GameInfo_t *game) {
  for (int i = 0; i < BLOCK_SIZE; i++) {
    free(game->next[i]);
  }
  for (int i = 0; i < HEIGHT + 3; i++) {
    free(game->field[i]);
  }
  free(game->next);
  free(game->field);
  game->next = NULL;
  game->field = NULL;
}
