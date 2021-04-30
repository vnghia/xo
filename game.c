#include "game.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

void init_game(game_t* game) {
  game->board = (player_t*)malloc(game->size * game->size * sizeof(player_t));
  reset_game(game);
}

void reset_game(game_t* game) {
  memset(game->board, '\0', game->size * game->size * sizeof(player_t));
  // for (int i = 0; i < game->size; ++i) {
  //   for (int j = 0; j < game->size; ++j) {
  //     game->board[i * game->size + j] = PLAYER_NULL;
  //   }
  // }
  game->current_player = PLAYER_X;
  game->state = STATE_RUNNING;
}

void update_current_player(game_t* game) {
  if (game->current_player == PLAYER_X) {
    game->current_player = PLAYER_O;
  } else if (game->current_player == PLAYER_O) {
    game->current_player = PLAYER_X;
  }
}

int check_win_condition(const game_t* game, const player_t* player) {
  int row_count = 0;
  int column_count = 0;
  int diag1_count = 0;
  int diag2_count = 0;
  int total_filled = 0;

  for (int i = 0; i < game->size; ++i) {
    for (int j = 0; j < game->size; ++j) {
      if (game->board[i * game->size + j] == *player) {
        row_count++;
      } else if (game->board[i * game->size + j] == PLAYER_NULL) {
        total_filled++;
        row_count = 0;
      } else {
        row_count = 0;
      }

      if (game->board[j * game->size + i] == *player) {
        column_count++;
      } else {
        column_count = 0;
      }

      if (row_count >= game->win || column_count >= game->win) {
        return 1;
      }
    }

    if (i > game->size - game->win) continue;

    for (int j = 0; j < game->size; ++j) {
      if (i + j >= game->size - 1) break;
      SDL_Log("Current cell: (%d, %d) = %d\nCurrent diag: %d", i + j, j,
              game->board[(i + j) * game->size + j], diag1_count);
      if (game->board[(i + j) * game->size + j] == *player) {
        diag1_count++;
      } else {
        diag1_count = 0;
      }

      if (diag1_count >= game->win) {
        return 1;
      }
    }

    // for (int sum = game->win - 1; sum <= game->size - game->win; ++sum) {
    //   if (sum <= i) continue;
    //   SDL_Log("Current cell: (%d, %d) = %d\nCurrent diag: %d", i, sum - i,
    //           game->board[i * game->size + (sum - i)], diag2_count);
    //   if (game->board[i * game->size + (sum - i)] == *player) {
    //     diag2_count++;
    //   } else {
    //     diag2_count = 0;
    //   }

    //   if (diag2_count >= game->win) {
    //     return 1;
    //   }
    // }
    if (i < game->win - 1) continue;
    for (int j = 0; j <= i; ++j) {
      if (game->board[j * game->size + (i - j)] == *player) {
        diag2_count++;
      } else {
        diag2_count = 0;
      }

      if (diag2_count >= game->win) {
        return 1;
      }
    }
  }

  if (total_filled == game->size * game->size) return -1;
  return 0;
}
