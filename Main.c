#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#include <ctype.h> //check if note digit

#define NOTES_FILE_PATH "SampleNotes.txt"
#define GREEN_NOTE_CODE 0b1000 //could maybe just do bit shifting
#define RED_NOTE_CODE 0b0100
#define YELLOW_NOTE_CODE 0b0010
#define BLUE_NOTE_CODE 0b0001

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

void addNote(noteInfo* note, noteInfo** headNote, noteInfo** tailNote)
{
    if((*headNote) == NULL)
    {
        (*headNote) = note;
    }
    else
    {
        (*tailNote) -> previousNote = note;
        note -> nextNote = (*tailNote);
    }
    (*tailNote) = note;
}

noteInfo* popNote(noteInfo** headNote, noteInfo** tailNote) //does not account for empty queue
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
    while((*headNote) != NULL)
    {
        popNote(headNote, tailNote);
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
        //mutex lock
        addNote(popNote(&headNoteFile, &tailNoteFile), &headNoteDisplay, &tailNoteDisplay);
        printf("here    %d     here\n", popNote(&headNoteDisplay, &tailNoteDisplay)->note);
        sleepForMs(currentNote->timeToNextNote);
        freeNote();
        //mutex unlock
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

    pthread_t noteAdder;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int* nullArg = 0;

    pthread_create(&noteAdder, &attr, readNoteToDisplayQueue, &nullArg);
    pthread_join(noteAdder, NULL);

    deleteNotes(&headNoteFile, &tailNoteFile);
    deleteNotes(&headNoteDisplay, &tailNoteDisplay);

    return 0;
}