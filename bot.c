#include "bot.h"

#include <stdlib.h>
#include <time.h>

#include "game.h"

int compute_bot_move(game_t* game) {
  static int is_seeded = 0;
  if (is_seeded == 0) {
    srand(time(NULL));
    is_seeded = 1;
  }
  for (int i = 0; i < game->size; ++i) {
    for (int j = 0; j < game->size; ++j) {
      int current_move = i * game->size + j;

      if (game->board[current_move] != PLAYER_NULL) continue;
      game->board[current_move] = game->current_player;

      player_t winner = check_win_condition(game);
      if (winner == game->current_player) {
        game->board[current_move] = PLAYER_NULL;
        return current_move;
      } else if (winner == PLAYER_NULL && game->state == STATE_TIE) {
        game->board[current_move] = PLAYER_NULL;
        return current_move;
      }

      update_current_player(game);
      game->board[current_move] = game->current_player;
      winner = check_win_condition(game);
      if (winner == game->current_player) {
        update_current_player(game);
        game->board[current_move] = PLAYER_NULL;
        game->state = STATE_RUNNING;
        return current_move;
      }
      update_current_player(game);

      game->board[current_move] = PLAYER_NULL;
    }
  }
  int move = rand() % (game->size * game->size);
  while (game->board[move] != PLAYER_NULL) {
    move = rand() % (game->size * game->size);
  }
  return move;
}
