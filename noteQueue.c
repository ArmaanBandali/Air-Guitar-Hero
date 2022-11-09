#include <pthread.h>
#include <ctype.h> //check if note is digit
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "noteQueue.h"
#include "utils.h"

#define NOTES_FILE_PATH "SampleNotes.txt"

void NoteQueue_addNote(noteInfo *note, noteInfo **headNote, noteInfo **tailNote)
{
    if ((*headNote) == NULL)
    {
        (*headNote) = note;
        (*headNote)->nextNote = NULL;
        (*headNote)->previousNote = NULL;
    }
    else
    {
        (*tailNote)->previousNote = note;
        note->nextNote = (*tailNote);
    }
    (*tailNote) = note;
}

// Return is redundant here
// Caller must ensure note is freed
noteInfo *NoteQueue_popNote(noteInfo **headNote, noteInfo **tailNote, noteInfo **currentNote)
{
    (*currentNote) = (*headNote);
    if ((*headNote) != (*tailNote))
    {
        (*headNote)->previousNote->nextNote = NULL;
        (*headNote) = (*headNote)->previousNote;
        (*currentNote)->previousNote = NULL;
    }
    else //this condition is if note is last note in queue
    {
        (*headNote) = NULL;
        (*tailNote) = NULL;
    }
    return *currentNote;
}

// Free is separate from pop to allow more flexibility
void NoteQueue_freeNote(noteInfo *currentNote)
{
    free(currentNote);
    currentNote = NULL;
}

void NoteQueue_deleteNotes(noteInfo **headNote, noteInfo **tailNote, noteInfo **currentNote)
{
    int i = 0;
    while ((*headNote) != NULL)
    {
        NoteQueue_popNote(headNote, tailNote, currentNote);
        printf("Deleting    %d     here   %d\n", (*currentNote)->note, i++);
        NoteQueue_freeNote(*currentNote);
    }

    (*currentNote) = NULL;
    (*headNote) = NULL;
    (*tailNote) = NULL;
}


//If this process is long, potential for desync. May need its own thread that all other threads must wait for
//ORs valid notes to a binary note code
//
int NoteQueue_loadNotesFromFile(noteInfo **headNote, noteInfo **tailNote)
{
    int timeToFirstNote = 0;

    FILE *pFile = fopen(NOTES_FILE_PATH, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", NOTES_FILE_PATH);
        exit(-1);
    }

    // Read string (line)
    const int MAX_LENGTH = 1024; //Assumes line sizes are less than 1024 characters
    char buff[MAX_LENGTH];
    while (fgets(buff, MAX_LENGTH, pFile))
    {
        printf("%s", buff);
        int noteCode = 0b0;
        int timeToNextNote = 0;
        const char NLINE = '\n';
        const char ENDNOTE = '$';
        const int ASCII_INT_OFFSET = 48;
        bool comment = false;

        if(buff[0] == NLINE) //Do not add note for empty lines
        {
            comment = true;
        }

        for (int i = 0; buff[i] != NLINE; i++)
        {
            if (isalpha(buff[i]))
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
                case 'O':
                {
                    noteCode = noteCode | ORANGE_NOTE_CODE;
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
                timeToNextNote *= 10; //shift each decimal digit left
                timeToNextNote += (buff[i] - ASCII_INT_OFFSET); //get value of int
            }
            else if (buff[i] == ENDNOTE) //ENDNOTE signifies last charcter before eof
            {
                if(i==0)
                {
                    comment = true;
                }
                break;
            }
            else
            {
                continue;
            }
        }
        if(!comment) //skip adding note if line was a comment
        {
            if(noteCode == 0)
            {
                timeToFirstNote = timeToNextNote;
            }
            else{
                noteInfo *newNote = malloc(sizeof(*newNote)); // WRITE INIT FUNCTION
                newNote->note = noteCode;
                newNote->timeToNextNote = timeToNextNote;
                NoteQueue_addNote(newNote, headNote, tailNote);
                newNote = NULL;
            }
        }
        else
        {
            comment = false;
        }
    }
    printf("\n");

    // Close
    fclose(pFile);
    return timeToFirstNote;
}