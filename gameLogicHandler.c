#include <stdbool.h>
#include <pthread.h>

#include "displayModel.h"
#include "noteQueue.h"
#include "buttonArray.h"

bool gameOver = false;
bool noteHit = false;

pthread_t gameLogicThread;
static pthread_attr_t attr;

static void *GameLogicHandler_startThread(void *arg)
{
    while(!gameOver) //should this sleep? would need to be closely coupled with displayModel+
    {
        noteInfo* activeNote = getHeadNoteDisplayQueue();
        if (activeNote == NULL) //displayModel joins before GameLogicHandler, so note may be null
        {
            continue; //could probably break here
        }
        int activeButtonValues[5] ={0, 0, 0, 0, 0};
        ButtonArray_getButtonValues(activeButtonValues);
        //TODO Compare int value of note to button values and update noteHit
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