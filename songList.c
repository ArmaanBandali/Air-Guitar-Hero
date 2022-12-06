#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "buttonArray.h"
#include "songList.h"
#include "gameLogicHandler.h"
#include "utils.h"
#include "ctype.h"

#define SONG_LIST_FILE "songList.txt"

static songInfo songList[MAX_SONG_LIMIT];

int songsLoaded = 0;

static gameState SongList_gameState;
static songInfo selectedSong;

static void SongList_loadSongsFromFile();
static void SongList_displaySongList();
static void SongList_selectSong();
static void SongList_songSelected();
static void SongList_resetModule();

// Uses comma delimiter
static void SongList_loadSongsFromFile()
{
    FILE *pFile = fopen(SONG_LIST_FILE, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", SONG_LIST_FILE);
        exit(-1);
    }

    
    size_t MAX_LENGTH = 0; //Change this?
    char* songListBuff = NULL;
    
    const char COMMA = ',';
    int whichString = 0;

    while (getdelim(&songListBuff, &MAX_LENGTH, COMMA, pFile) != -1) //TODO: THis is jank central
    {
        if (whichString%4 == 0)
        {
            if(whichString != 0)
            {
                songsLoaded++;
            }
            strcpy(songList[songsLoaded].songName, &songListBuff[1]);
            songList[songsLoaded].songName[strlen(songList[songsLoaded].songName)-1] = 0;
        }
        else if (whichString%4 == 1)
        {
            strcpy(songList[songsLoaded].audioFileLocation, songListBuff);
            songList[songsLoaded].audioFileLocation[strlen(songList[songsLoaded].audioFileLocation) - 1] = 0;
        }
        else if (whichString%4 == 2)
        {
            strcpy(songList[songsLoaded].leaderboardFileLocation, songListBuff);
            songList[songsLoaded].leaderboardFileLocation[strlen(songList[songsLoaded].leaderboardFileLocation) - 1] = 0;
        }
        else if (whichString%4 == 3)
        {
            strcpy(songList[songsLoaded].notesFileLocation, songListBuff);
            songList[songsLoaded].notesFileLocation[strlen(songList[songsLoaded].notesFileLocation) - 1] = 0;
        }

        whichString++;
    }
    free(songListBuff);
    songListBuff = NULL;
    fclose(pFile);
}

void SongList_manageSongList(gameState state)
{
    SongList_setGameState(state);
    SongList_loadSongsFromFile();
    SongList_displaySongList();
    SongList_selectSong();
    SongList_songSelected();
    SongList_resetModule();
}

static void SongList_displaySongList()
{
    // Placeholder code for display
    printf("Song List\n\nSelect a song by entering its number\n");
    for(int i=0; i<songsLoaded; i++)
    {
        printf("%d)  %s\n\n",i+1,songList[i].songName);
    }
}

static void SongList_selectSong()
{
    //TODO Check Button
    //TODO Check Strum

    //Placeholder for display
    char songChoice;
    bool songFound = false;
    const int ASCII_INT_OFFSET = 48;
    do
    {
        scanf("%c", &songChoice);
        fflush(stdin);
        int numberSelection = songChoice - ASCII_INT_OFFSET;
        if (numberSelection > songsLoaded || !isdigit(songChoice))
        {
            printf("Invalid song choice: Enter the number of the song you want to play\n\n");
            SongList_displaySongList();
        }
        else
        {
            songFound = true;
            selectedSong = songList[numberSelection-1];
        }
    } while (!songFound);

}

static void SongList_songSelected()
{
    if(SongList_getGameState() == LEADERBOARD_MODE)
    {
        //TODO launch leaderboard
    }
    else if(SongList_getGameState() == SONG_SELECT)
    {
        GameLogicHandler_startLogicHandler(selectedSong); 
    }
}

//Manage anything that was dynamically allocated
static void SongList_resetModule()
{
}

void SongList_setGameState(gameState state)
{
    SongList_gameState = state;
}

gameState SongList_getGameState()
{
    return SongList_gameState;
}