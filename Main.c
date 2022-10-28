#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>

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

noteInfo* headNote = NULL;
noteInfo* tailNote = NULL;
noteInfo* currentNote = NULL;

void addNote(noteInfo* note)
{
    if(headNote == NULL)
    {
        headNote = note;
    }
    else
    {
        tailNote -> previousNote = note;
        note -> nextNote = tailNote;
    }
    tailNote = note;
}

noteInfo* popNote() //does not account for empty queue
{
    currentNote = headNote;
    if (headNote != tailNote)
    {
        headNote -> previousNote -> nextNote = NULL;
        headNote = headNote -> previousNote;
        currentNote -> previousNote = NULL;
    }
    return currentNote;
}

void freeNote()
{
    free(currentNote);
}

void deleteNotes()
{
    while(headNote != tailNote)
    {
        popNote();
        freeNote();
    }
    popNote();
    freeNote();

    currentNote = NULL;
    headNote = NULL;
    tailNote = NULL;
}




void* readNote(void* _)
{

}

void* writeNote(void* _)
{
    
}

void* updateDisplay(void* _)
{
    
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
        addNote(newNote);
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

int main()
{
    loadNotesFromFile();

    printf("here    %d     here\n", popNote()->note);
    freeNote();
    printf("here    %d     here\n", popNote()->note);
    freeNote();
    printf("here    %d     here\n", popNote()->note);
    freeNote();

    //deleteNotes();

    return 0;
}