#ifndef SONGLIST
#define SONGLIST

#define MAX_SONG_LIMIT 20

typedef enum {
    SONG_SELECT,
    LEADERBOARD_MODE
}gameState;

typedef struct {
    char songName[100];
    char audioFileLocation[100];
    char leaderboardFileLocation[100];
    char notesFileLocation[100];
}songInfo;

void SongList_manageSongList(gameState state);

void SongList_setGameState(gameState state);
gameState SongList_getGameState();

#endif