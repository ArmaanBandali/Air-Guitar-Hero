#include <stdlib.h>
#include <stdio.h>

#include "accelerometer.h"

int main(int argc, char **argv)
{
    accelerometer_init();

    while(1) {
        // wait
    }

    accelerometer_cleanup();

    printf("\nFinal score: %d\n", GameLogicHandler_getScore());
    return 0;
}