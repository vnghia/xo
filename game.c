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

player_t check_win_condition(game_t* game) {
  int x_row_count = 0;
  int x_column_count = 0;

  int o_row_count = 0;
  int o_column_count = 0;

  int x_diag11_count = 0;
  int x_diag12_count = 0;
  int x_diag21_count = 0;
  int x_diag22_count = 0;

  int o_diag11_count = 0;
  int o_diag12_count = 0;
  int o_diag21_count = 0;
  int o_diag22_count = 0;

  int total_filled = 0;

  for (int i = 0; i < game->size; ++i) {
    for (int j = 0; j < game->size; ++j) {
      switch (game->board[i * game->size + j]) {
        case PLAYER_X:
          x_row_count++;
          o_row_count = 0;
          break;
        case PLAYER_O:
          x_row_count = 0;
          o_row_count++;
          break;
        default:
          total_filled++;
          x_row_count = 0;
          o_row_count = 0;
          break;
      }

      switch (game->board[j * game->size + i]) {
        case PLAYER_X:
          x_column_count++;
          o_column_count = 0;
          break;
        case PLAYER_O:
          x_column_count = 0;
          o_column_count++;
          break;
        default:
          x_column_count = 0;
          o_column_count = 0;
          break;
      }

      if (x_row_count >= game->win || x_column_count >= game->win) {
        game->state = STATE_WINNING;
        return PLAYER_X;
      }

      if (o_row_count >= game->win || o_column_count >= game->win) {
        game->state = STATE_WINNING;
        return PLAYER_O;
      }

      if (i + j > game->size - 1) break;

      int last_index = game->size - 1;

      switch (game->board[(i + j) * game->size + j]) {
        case PLAYER_X:
          x_diag11_count++;
          o_diag11_count = 0;
          break;
        case PLAYER_O:
          x_diag11_count = 0;
          o_diag11_count++;
          break;
        default:
          x_diag11_count = 0;
          o_diag11_count = 0;
          break;
      }

      switch (game->board[(i + j) * game->size + last_index - j]) {
        case PLAYER_X:
          x_diag21_count++;
          o_diag21_count = 0;
          break;
        case PLAYER_O:
          x_diag21_count = 0;
          o_diag21_count++;
          break;
        default:
          x_diag21_count = 0;
          o_diag21_count = 0;
          break;
      }

      if (x_diag11_count >= game->win || x_diag21_count >= game->win) {
        game->state = STATE_WINNING;
        return PLAYER_X;
      }

      if (o_diag11_count >= game->win || o_diag21_count >= game->win) {
        game->state = STATE_WINNING;
        return PLAYER_O;
      }

      if (i == 0) continue;

      switch (game->board[j * game->size + (i + j)]) {
        case PLAYER_X:
          x_diag12_count++;
          o_diag12_count = 0;
          break;
        case PLAYER_O:
          x_diag12_count = 0;
          o_diag12_count++;
          break;
        default:
          x_diag12_count = 0;
          o_diag12_count = 0;
          break;
      }

      switch (game->board[(last_index - i - j) * game->size + j]) {
        case PLAYER_X:
          x_diag22_count++;
          o_diag22_count = 0;
          break;
        case PLAYER_O:
          x_diag22_count = 0;
          o_diag22_count++;
          break;
        default:
          x_diag22_count = 0;
          o_diag22_count = 0;
          break;
      }

      if (x_diag12_count >= game->win || x_diag22_count >= game->win) {
        game->state = STATE_WINNING;
        return PLAYER_X;
      }

      if (o_diag12_count >= game->win || o_diag22_count >= game->win) {
        game->state = STATE_WINNING;
        return PLAYER_O;
      }
    }

    x_row_count = 0;
    x_column_count = 0;

    o_row_count = 0;
    o_column_count = 0;

    x_diag11_count = 0;
    x_diag12_count = 0;
    x_diag21_count = 0;
    x_diag22_count = 0;

    o_diag11_count = 0;
    o_diag12_count = 0;
    o_diag21_count = 0;
    o_diag22_count = 0;
  }

  if (total_filled == game->size * game->size) game->state = STATE_TIE;
  return PLAYER_NULL;
}
