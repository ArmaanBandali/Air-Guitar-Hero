#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "displayModel.h"
#include "buttonArray.h"
#include "songList.h"

#define STATUS_QUIT 0
#define STATUS_CONTINUE 1


static int printHomeScreen();
//static void selectMenuItem();
static void cleanup();
static void *runGameInstance(void *_);

typedef enum {
    SONG_SELECTION,
    LEADERBOARDS,
    END_OF_MENU
} gameMenuItems;

gameMenuItems selectedMenuItem = SONG_SELECTION;

pthread_t gameInstanceThread;
static pthread_attr_t attr;

int main(int argc, char **argv)
{

    ButtonArray_start();
    //TODO Start Strum
    //TODO Start Display

    pthread_attr_init(&attr);

    while(true)
    {
        pthread_create(&gameInstanceThread, &attr, runGameInstance, NULL);

        void *returnStatus;
        pthread_join(gameInstanceThread, &returnStatus);

        if (((int)returnStatus) == STATUS_QUIT)
        {
            cleanup();
            break;
        }
        else if (((int)returnStatus) == STATUS_CONTINUE)
        {
            continue;
        }
    }

    return 0;
}

static void* runGameInstance(void*_)
{
    int returnCode = printHomeScreen();
    return (void*)returnCode;
}

static int printHomeScreen()
{
    //TODO Write home screen to display
    printf("Air Guitar Hero\n\n");
    printf("Main Menu:\n");
    printf("    Song Select\n");
    printf("    Leaderboards\n");
    printf("    Quit\n");

    char choice;

    scanf(" %c", &choice);
    fflush(stdin);
    while (choice != 'a' && choice != 'b' && choice != 'q') // Placeholder using a and b for song select and leaderboard
    {
        printf("Invalid choice: Please enter a,b, or q\n\n");
        printf("Air Guitar Hero\n\n");
        printf("Main Menu:\n");
        printf("    Song Select\n");
        printf("    Leaderboards\n");
        printf("    Quit\n");
        scanf(" %c", &choice);
        fflush(stdin);
    }

    switch (choice)
    {
    case 'a':
        //Launch song list with state
        SongList_manageSongList(SONG_SELECTION);

        break;
    case 'b':
        SongList_manageSongList(LEADERBOARDS);
        break;
    case 'q':
        return STATUS_QUIT;

    default:
        break;
    }

    return STATUS_CONTINUE;
}

static void cleanup()
{
    ButtonArray_stop();
    //TODO stop other threads
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