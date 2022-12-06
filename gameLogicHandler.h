// Module for running game logic thread for observing buttons, strum, notes, and updating score

#ifndef GAME_LOGIC_HANDLER
#define GAME_LOGIC_HANDLER

#include "songList.h"

void GameLogicHandler_startLogicHandler(songInfo selectedSong);
void GameLogicHandler_stopLogicHandler();
int GameLogicHandler_getScore();


#endif