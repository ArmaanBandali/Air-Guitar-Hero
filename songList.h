#define MAX_SONG_LIMIT 20

typedef enum {
    SONG_SELECT,
    LEADERBOARD
}gameState;

typedef struct {
    char* songName;
    char* audioFileLocation;
    char* leaderboardFileLocation;
    char* notesFileLocation;
}songInfo;

void SongList_manageSongList(gameState state);

void SongList_setGameState(gameState state);
gameState SongList_getGameState();