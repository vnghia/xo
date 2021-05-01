#include <SDL.h>
#include <stdlib.h>

#include "bot.h"
#include "game.h"
#include "rendering.h"

int main(int argc, char *argv[]) {
  // init sdl

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("Tic Tac Toe", 100, 100, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  // game parameters

  game_t game;
  game.size = 20;
  game.win = 4;
  game.bot = 1;
  const int CELL_HEIGHT = (int)(SCREEN_HEIGHT / game.size);
  const int CELL_WIDTH = (int)(SCREEN_WIDTH / game.size);
  init_game(&game);
  SDL_Event e;

  // main loop
  do {
    render_board(renderer, &game, CELL_HEIGHT, CELL_WIDTH);
    SDL_RenderPresent(renderer);

    while (game.state == STATE_RUNNING) {
      if (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) {
          game.state = STATE_QUIT;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
          // click and render

          int cell_index = (int)(e.button.y / CELL_HEIGHT) * game.size +
                           (int)(e.button.x / CELL_WIDTH);
          if (game.board[cell_index] != PLAYER_NULL) continue;

          game.board[cell_index] = game.current_player;
          render_board(renderer, &game, CELL_HEIGHT, CELL_WIDTH);
          SDL_RenderPresent(renderer);

          // check win
          if (check_win_condition(&game) == PLAYER_NULL &&
              game.state != STATE_TIE) {
            update_current_player(&game);
            if (game.bot != 0) {
              int move = compute_bot_move(&game);
              SDL_Log("bot move: %d", move);
              game.board[move] = game.current_player;
              render_board(renderer, &game, CELL_HEIGHT, CELL_WIDTH);
              SDL_RenderPresent(renderer);
              if (game.state == STATE_RUNNING) {
                update_current_player(&game);
              }
            }
          }
        }
      }
    }

    if (game.state == STATE_WINNING) {
      const SDL_MessageBoxButtonData buttons[] = {
          {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Replay"},
          {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Exit"},
      };
      const SDL_MessageBoxColorScheme color_scheme = {{{255, 0, 0},
                                                       {0, 255, 0},
                                                       {255, 255, 0},
                                                       {0, 0, 255},
                                                       {255, 0, 255}}};
      SDL_MessageBoxData message_box_data = {SDL_MESSAGEBOX_INFORMATION,
                                             NULL,
                                             NULL,
                                             "Please choose one option",
                                             SDL_arraysize(buttons),
                                             buttons,
                                             &color_scheme};
      if (game.current_player == PLAYER_X) {
        message_box_data.title = "Congratulation Player X";
      } else if (game.current_player == PLAYER_O) {
        message_box_data.title = "Congratulation Player O";
      }
      int button_id;
      SDL_ShowMessageBox(&message_box_data, &button_id);
      if (button_id == 0) {
        reset_game(&game);
      } else {
        game.state = STATE_QUIT;
      }
    }
  } while (game.state != STATE_QUIT);

  // cleanup

  free(game.board);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
