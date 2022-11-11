#include <stdlib.h>
#include <stdio.h>

#include "displayModel.h"
#include "gameLogicHandler.h"

int main(int argc, char **argv)
{
    DisplayModel_startDisplayModel();
    GameLogicHandler_startLogicHandler();

    DisplayModel_stopDisplayModel();
    GameLogicHandler_stopLogicHandler();

    printf("\nFinal score: %d\n", GameLogicHandler_getScore());
    return 0;
}