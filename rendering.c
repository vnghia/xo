#include "rendering.h"

#include <SDL.h>

#include "game.h"

const SDL_Color GRID_COLOR = {.r = 255, .g = 255, .b = 255};
const SDL_Color PLAYER_X_COLOR = {.r = 255, .g = 50, .b = 50};
const SDL_Color PLAYER_O_COLOR = {.r = 0, .g = 0, .b = 255};

void render_grid(SDL_Renderer *renderer, int nrow, int ncol, double cell_height,
                 double cell_width, const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

  for (int i = 1; i < nrow; ++i) {
    SDL_RenderDrawLine(renderer, 0, i * cell_height, SCREEN_WIDTH,
                       i * cell_height);
  }

  for (int i = 1; i < ncol; ++i) {
    SDL_RenderDrawLine(renderer, i * cell_width, 0, i * cell_width,
                       SCREEN_WIDTH);
  }
}

void render_x(SDL_Renderer *renderer, int row, int col, double cell_height,
              double cell_width, const SDL_Color *color) {
  const int half_box_side = (int)(fmin(cell_width, cell_height) * 0.25);
  const int center_x = (int)(cell_width * 0.5 + col * cell_width);
  const int center_y = (int)(cell_height * 0.5 + row * cell_height);

  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

  SDL_RenderDrawLine(renderer, center_x - half_box_side,
                     center_y - half_box_side, center_x + half_box_side,
                     center_y + half_box_side);
  SDL_RenderDrawLine(renderer, center_x + half_box_side,
                     center_y - half_box_side, center_x - half_box_side,
                     center_y + half_box_side);
}

void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY,
                 int32_t radius) {
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

void render_o(SDL_Renderer *renderer, int row, int col, double cell_height,
              double cell_width, const SDL_Color *color) {
  const int half_box_side = (int)(fmin(cell_width, cell_height) * 0.25);
  const int center_x = (int)(cell_width * 0.5 + col * cell_width);
  const int center_y = (int)(cell_height * 0.5 + row * cell_height);

  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

  draw_circle(renderer, center_x, center_y, half_box_side);
}

void render_players(SDL_Renderer *renderer, game_t *game, double cell_height,
                    double cell_width) {
  for (int i = 0; i < game->size; ++i) {
    for (int j = 0; j < game->size; ++j) {
      player_t *current_cell = game->board + i * game->size + j;
      if (*current_cell == PLAYER_X) {
        render_x(renderer, i, j, cell_height, cell_width, &PLAYER_X_COLOR);
      } else if (*current_cell == PLAYER_O) {
        render_o(renderer, i, j, cell_height, cell_width, &PLAYER_O_COLOR);
      }
    }
  }
}

void render_board(SDL_Renderer *renderer, game_t *game, double cell_height,
                  double cell_width) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  render_grid(renderer, game->size, game->size, cell_height, cell_width,
              &GRID_COLOR);
  render_players(renderer, game, cell_height, cell_width);
}
