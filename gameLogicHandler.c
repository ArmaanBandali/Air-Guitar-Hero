#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "displayModel.h"
#include "noteQueue.h"
#include "buttonArray.h"
#include "songList.h"

bool gameOver = false;
bool noteHit = false;
int score = 0;

pthread_t gameLogicThread;
static pthread_attr_t attr;

static void *GameLogicHandler_startThread(void *arg)
{
    int lastNoteValue = 0;
    bool noteAlreadyHit = false;
    while(!gameOver) //should this sleep? would need to be closely coupled with displayModel
    {
        int activeNoteValue = 0;

        DisplayQueue_lockDisplayQueueMutex();
        {
            if (DisplayQueue_getHeadNoteDisplayQueue() == NULL) // displayModel joins before GameLogicHandler, so note may be null
            {
                continue; // could probably break here
            }
            activeNoteValue = DisplayQueue_getHeadNoteDisplayQueue()->note;
        }
        DisplayQueue_unlockDisplayQueueMutex();

        int buttonNoteValue = 0;

        ButtonArray_lockButtonMutex();
        {
            buttonNoteValue = ButtonArray_getCurrentButtonValue();
        }
        ButtonArray_unlockButtonMutex();
        
        if (noteAlreadyHit && lastNoteValue != activeNoteValue)
        {
            noteAlreadyHit = false;
        }
        if (activeNoteValue == buttonNoteValue && !noteAlreadyHit && activeNoteValue != 0) //check if note is correct and not already hit
        {
            score++;
            printf("\n=================\nHooray!\n==================\n");
            noteAlreadyHit = true;
        }
        lastNoteValue = activeNoteValue;
    }
}

void GameLogicHandler_startLogicHandler(songInfo selectedSong)
{
    DisplayModel_startDisplayModel(selectedSong);
    pthread_attr_init(&attr);
    pthread_create(&gameLogicThread, &attr, GameLogicHandler_startThread, NULL);

    //Blocks until song finishes
    DisplayModel_stopDisplayModel();
    GameLogicHandler_stopLogicHandler();
    
    //TODO Write to leaderboard file
}

void GameLogicHandler_stopLogicHandler()
{
    gameOver = true;
    pthread_join(gameLogicThread, NULL);
}

int GameLogicHandler_getScore()
{
    return score;
}