#include <stdio.h>
#include <stdbool.h>

#include "displayModel.h"
#include "buttonArray.h"
#include "songList.h"

static void printHomeScreen();
static void selectMenuItem();
static void cleanup();

typedef enum{
    SONG_SELECT,
    LEADERBOARDS,
    END_OF_MENU
}gameMenuItems;

gameMenuItems selectedMenuItem = SONG_SELECT;

int main(int argc, char **argv)
{

    ButtonArray_start();
    //TODO Start Strum
    //TODO Start Display

    printHomeScreen();

    while (true)
    {
        // Quit?
        if (toupper(getchar()) == 'Q')
        {
            break;
        }
    }
    cleanUp();
    return 0;
}

static void printHomeScreen()
{
    //TODO Write home screen to display
    printf("Air Guitar Hero\n\n");
    printf("Main Menu:\n");
    printf("    Song Select\n");
    printf("    Leaderboards");

    char choice;

    while (scanf("%c", choice) != 'a' || scanf("%c", choice) != 'b') //Placeholder using a and b for song select and leaderboard
    {
    }

    switch (choice)
    {
    case 'a':
        //Launch song list with state
        SongList_manageSongList(SONG_SELECT);

        break;
    case 'b':
        SongList_manageSongList(LEADERBOARD);
        break;

    default:
        break;
    }

    //TODO handle logic here for after song list activity
}

static void cleanup()
{
    ButtonArray_stop();
}

void resetModule()
{
    //TODO reset module after returning to main menu
    //Need similar resets for gameLogicHandler and displayModel
}

// static void selectMenuItem()
// {
//     while(true) //Loop until item is selected
//     {
//         // TODO Check button
//         {
//             startMenuItem();
//             break;
//         }
//         // TODO Check Strum
//         {
//             if (++selectedMenuItem == END_OF_MENU)
//             {
//                 selectedMenuItem = SONG_SELECT;
//             }
//         }
//     }
// }

// static void startMenuItem()
// {
//     switch (selectedMenuItem)
//     {
//     case SONG_SELECT:
//         //TODO Start song select
//         break;

//     case LEADERBOARDS:
//         //TODO Start song select
//         break;

//     default:
//         break;
//     }
// }