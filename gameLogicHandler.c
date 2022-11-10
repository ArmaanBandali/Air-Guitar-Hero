#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "displayModel.h"
#include "noteQueue.h"
#include "buttonArray.h"

bool gameOver = false;
bool noteHit = false;
int score = 0;

pthread_t gameLogicThread;
static pthread_attr_t attr;

static void *GameLogicHandler_startThread(void *arg)
{
    while(!gameOver) //should this sleep? would need to be closely coupled with displayModel+
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
        int activeButtonValues[5] ={0, 0, 0, 0, 0};
        ButtonArray_getButtonValues(activeButtonValues);
        int buttonNoteValue = ButtonArray_buttonValuesToNote(activeButtonValues);
    
        if (activeNoteValue == buttonNoteValue)
        {
            score++;
            printf("\n=================\nHooray!\n==================\n");
        }
    }
}

void GameLogicHandler_startLogicHandler()
{
    ButtonArray_initializeButtons();
    pthread_attr_init(&attr);
    pthread_create(&gameLogicThread, &attr, GameLogicHandler_startThread, NULL);
}

void GameLogicHandler_stopLogicHandler()
{
    gameOver = true;
    pthread_join(gameLogicThread, NULL);
}