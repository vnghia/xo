#ifndef TIC_TAC_TOE_GAME_H_
#define TIC_TAC_TOE_GAME_H_

#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0

typedef enum player_t { PLAYER_NULL = 0, PLAYER_X, PLAYER_O } player_t;
typedef enum state_t {
  STATE_INIT,
  STATE_RUNNING,
  STATE_WINNING,
  STATE_TIE,
  STATE_QUIT
} state_t;

typedef struct game_t {
  int size;
  int win;
  int bot;
  player_t* board;
  player_t current_player;
  state_t state;
} game_t;

void init_game(game_t* game);
void reset_game(game_t* game);
void update_current_player(game_t* game);

player_t check_win_condition(game_t* game);

#endif  // TIC_TAC_TOE_GAME_H_
