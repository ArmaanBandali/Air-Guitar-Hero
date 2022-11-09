#include <stdlib.h>

#include "displayModel.h"
#include "gameLogicHandler.h"

int main(int argc, char **argv)
{
    DisplayModel_startDisplayModel();
    GameLogicHandler_startLogicHandler();

    DisplayModel_stopDisplayModel();
    GameLogicHandler_stopLogicHandler();

    return 0;
}