#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include "ButtonArray.h"
#include <ctype.h> //check if note is digit

#define NOTES_FILE_PATH "SampleNotes.txt"
#define NO_NOTE_CODE 0b0000
#define GREEN_NOTE_CODE 0b1000 //could maybe just do bit shifting
#define RED_NOTE_CODE 0b0100
#define YELLOW_NOTE_CODE 0b0010
#define BLUE_NOTE_CODE 0b0001
#define FRAME_RATE 300 //needs to be experimentally defined

bool notesFinished = false;

typedef struct NoteInfo { 
    int note;
    int timeToNextNote;
    struct NoteInfo* nextNote;
    struct NoteInfo* previousNote;
} noteInfo;

noteInfo* headNoteFile = NULL;
noteInfo* tailNoteFile = NULL;

noteInfo* headNoteDisplay = NULL;
noteInfo* tailNoteDisplay = NULL;

noteInfo* currentNote = NULL;

pthread_mutex_t displayQueueMutex = PTHREAD_MUTEX_INITIALIZER;

//Dr. Brian's function
//Program sleeps for specified time in ms
static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

//Dr. Brian's function
//Return current Unix time in ms
static long long getTimeInMs(void)
{
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	long long seconds = spec.tv_sec;
	long long nanoSeconds = spec.tv_nsec;
	long long milliSeconds = seconds * 1000
	+ nanoSeconds / 1000000;
	return milliSeconds;
}

void addNote(noteInfo* note, noteInfo** headNote, noteInfo** tailNote)
{
    if((*headNote) == NULL)
    {
        (*headNote) = note;
        (*headNote) -> nextNote = NULL;
        (*headNote) -> previousNote = NULL;
    }
    else
    {
        (*tailNote) -> previousNote = note;
        note -> nextNote = (*tailNote);
    }
    (*tailNote) = note;
}

noteInfo* popNote(noteInfo** headNote, noteInfo** tailNote)
{
    currentNote = (*headNote);
    if ((*headNote) != (*tailNote))
    {
        (*headNote) -> previousNote -> nextNote = NULL;
        (*headNote) = (*headNote) -> previousNote;
        currentNote -> previousNote = NULL; //this is a problem need 2
    }
    else
    {
        (*headNote) = NULL;
        (*tailNote) = NULL;
    }
    return currentNote;
}

void freeNote()
{
    free(currentNote);
}

void deleteNotes(noteInfo** headNote, noteInfo** tailNote)
{
    int i = 0;
    while((*headNote) != NULL)
    {
        popNote(headNote, tailNote);
        printf("Deleting    %d     here   %d\n", currentNote->note, i++);
        freeNote();
    }

    currentNote = NULL;
    (*headNote) = NULL;
    (*tailNote) = NULL;
}

void* readNoteToDisplayQueue(void* arg)
{
    while(headNoteFile != NULL)
    {
        pthread_mutex_lock(&displayQueueMutex);
        {
            addNote(popNote(&headNoteFile, &tailNoteFile), &headNoteDisplay, &tailNoteDisplay);
        }
        pthread_mutex_unlock(&displayQueueMutex);
        sleepForMs(currentNote->timeToNextNote);
    }
    notesFinished = true;
}

void readDisplay()
{
    noteInfo* it = tailNoteDisplay;
    int i = 0;
    printf("===============================\n\n");
    while(it != NULL)
    {
        printf("Reading    %d     here   %d\n", it->note, i++);
        it = it->nextNote;
    }
    printf("===============================\n\n");
}

//TODO: frame rate should be decoupled from adding spacer
//      score, button, and strum checking should be done in this thread
void* addSpacerNote(void* arg)
{
    long long startTimeSpacer = getTimeInMs();
    while(headNoteDisplay != NULL)
    {
        if(getTimeInMs()-startTimeSpacer > FRAME_RATE)
        {
            startTimeSpacer = getTimeInMs();
            pthread_mutex_lock(&displayQueueMutex);
            {
                readDisplay();
                if(!notesFinished)
                {
                    noteInfo* newNote = malloc(sizeof(*newNote)); //WRITE INIT FUNCTION
                    newNote -> note = NO_NOTE_CODE;
                    newNote -> timeToNextNote = 0;
                    addNote(newNote, &headNoteDisplay, &tailNoteDisplay);
                    newNote = NULL;
                }
                popNote(&headNoteDisplay, &tailNoteDisplay);
                freeNote();
            }
            pthread_mutex_unlock(&displayQueueMutex);  
        }
    }
}

void* eraseNote(void* _)
{

}

void* updateDisplay(void* _)
{
    //refresh display
}

void* convertToDisplay(void* _)
{

}

void loadNotesFromFile()
{
    FILE *pFile = fopen(NOTES_FILE_PATH, "r");
    if (pFile == NULL) 
	{
		printf("ERROR: Unable to open file (%s) for read\n", NOTES_FILE_PATH);
		exit(-1);
	}

    // Read string (line)
	const int MAX_LENGTH = 1024;
	char buff[MAX_LENGTH];
    while (fgets(buff, MAX_LENGTH, pFile))
    {
        printf("%s",buff);
        int noteCode = 0b0;
        int timeToNextNote = 0;
        const char NLINE = '\n';
        const char ENDNOTE = '$';
        const int ASCII_INT_OFFSET = 48;

        for(int i=0; buff[i] != NLINE; i++)
        {
            if(isalpha(buff[i]))
            {
                switch (buff[i])
                {
                    case 'G':
                    {
                        noteCode = noteCode | GREEN_NOTE_CODE;
                        break;
                    }
                    case 'R':
                    {
                        noteCode = noteCode | RED_NOTE_CODE;
                        break;
                    }
                    case 'Y':
                    {
                        noteCode = noteCode | YELLOW_NOTE_CODE;
                        break;
                    }
                    case 'B':
                    {
                        noteCode = noteCode | BLUE_NOTE_CODE;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else if (isdigit(buff[i]))
            {
                timeToNextNote *= 10;
                timeToNextNote += (buff[i]-ASCII_INT_OFFSET);
            }
            else if (buff[i] == ENDNOTE)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        noteInfo* newNote = malloc(sizeof(*newNote)); //WRITE INIT FUNCTION
        newNote -> note = noteCode;
        newNote -> timeToNextNote = timeToNextNote;
        addNote(newNote, &headNoteFile, &tailNoteFile);
        newNote = NULL;
    }
    printf("\n");    

	// Close
	fclose(pFile);
}


void checkButton()
{

}

void checkStrum()
{

}

void updateScore()
{

}

int main(int argc, char **argv)
{
    loadNotesFromFile();

    //Follows Dr. Brian's notes on threads
    pthread_t noteAdder;
    pthread_t spaceAdder;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int* nullArg = 0;

    for(int i=0; i < 17; i++)
    {
        noteInfo* newNote = malloc(sizeof(*newNote)); //WRITE INIT FUNCTION
        newNote -> note = NO_NOTE_CODE;
        newNote -> timeToNextNote = 0;
        addNote(newNote, &headNoteDisplay, &tailNoteDisplay);
        newNote = NULL;
    }

    pthread_create(&noteAdder, &attr, readNoteToDisplayQueue, &nullArg);
    pthread_create(&spaceAdder, &attr, addSpacerNote, &nullArg);
    pthread_join(noteAdder, NULL);
    pthread_join(spaceAdder, NULL);

    deleteNotes(&headNoteFile, &tailNoteFile); //safety
    readDisplay();
    deleteNotes(&headNoteDisplay, &tailNoteDisplay); //safety

    return 0;
}