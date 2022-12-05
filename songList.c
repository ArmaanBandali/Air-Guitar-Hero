#include <stdio.h>
#include <stdbool.h>

#include "buttonArray.h"
#include "songList.h"
#include "gameLogicHandler.h"

#define SONG_LIST_FILE ""

static songInfo songList[MAX_SONG_LIMIT];

int songsLoaded = 0;

static gameState SongList_gameState;
static songInfo selectedSong;

static void SongList_loadSongsFromFile();
static void SongList_displaySongList();
static void SongList_selectSong();
static void SongList_songSelected();

//Uses comma delimiter
static void SongList_loadSongsFromFile()
{
    FILE *pFile = fopen(SONG_LIST_FILE, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", SONG_LIST_FILE);
        exit(-1);
    }

    // Read string (line)
    const int MAX_LENGTH = 1024; // Assumes line sizes are less than 1024 characters
    char buff[MAX_LENGTH];
    while (fgets(buff, MAX_LENGTH, pFile))
    {
        const char NLINE = '\n';
        const char COMMA = ',';
        const char SPACE = ' ';
        char* songName = "";
        char* audioFileLocation = "";
        char* leaderboardFileLocation = "";
        char* notesFileLocation = "";

        if(buff[0] == NLINE)
        {
            continue;
        }
        int whichString = 0; //Switch to next songInfo field
        for (int i = 0; buff[i] != NLINE; i++)
        {
            if(buff[i] == SPACE)
            {
                continue;
            }
            if(buff[i] != COMMA)
            {
                if(whichString == 0)
                {
                    songName+=buff[i];
                }
                else if(whichString == 1)
                {
                    audioFileLocation += buff[i];
                }
                else if (whichString == 2)
                {
                    leaderboardFileLocation += buff[i];
                }
                else if (whichString == 3)
                {
                    notesFileLocation += buff[i];
                }
            }
            else
            {
                whichString++;
            }
        }

        songList[songsLoaded].songName = songName;
        songList[songsLoaded].audioFileLocation = audioFileLocation;
        songList[songsLoaded].leaderboardFileLocation = leaderboardFileLocation;
        songList[songsLoaded].notesFileLocation = notesFileLocation;
        songsLoaded++;
    }

    fclose(pFile);
}

void SongList_manageSongList(gameState state)
{
    SongList_setGameState(state);
    SongList_displaySongList();
    SongList_selectSong();
    SongList_songSelected();
    SongList_resetModule();
}

static void SongList_displaySongList()
{
    // Placeholder code for display
    printf("Song List\n\nSelect a song by typing its name\n");
    for(int i=0; i<songsLoaded; i++)
    {
        printf("%s\n",songList[i].songName);
    }
}

static void SongList_selectSong()
{
    //TODO Check Button
    //TODO Check Strum

    //Placeholder for display
    char* songChoice;
    bool songFound = false;
    do
    {
        scanf("%s", songChoice);
        for (int i = 0; i < songsLoaded; i++)
        {
            if(songList[i].songName == songChoice)
            {
                songFound = true;
                selectedSong = songList[i];
            }
        }
        if(!songFound)
        {
            printf("Invalid song choice: Type the name of the song you want to play\n\n");
            SongList_displaySongList();
        }
    } while (!songFound);

}

static void SongList_songSelected()
{
    if(SongList_getGameState == LEADERBOARD)
    {
        //TODO launch leaderboard
    }
    else if(SongList_getGameState == SONG_SELECT)
    {
        GameLogicHandler_startLogicHandler(selectedSong); 
    }
}

//Manage anything that was dynamically allocated
void SongList_resetModule()
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