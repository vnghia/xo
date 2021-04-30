#ifndef TIC_TAC_TOE_RENDERING_H_
#define TIC_TAC_TOE_RENDERING_H_

#include <SDL.h>

#include "game.h"

void render_grid(SDL_Renderer *renderer, int nrow, int ncol, double cell_height,
                 double cell_width, const SDL_Color *color);

void render_board(SDL_Renderer *renderer, game_t *game, double cell_height,
                  double cell_width);

#endif  // TIC_TAC_TOE_RENDERING_H_
