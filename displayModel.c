#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "displayModel.h"
#include "noteQueue.h"
#include "utils.h"

noteInfo *headNoteDisplayQueue = NULL;
noteInfo *tailNoteDisplayQueue = NULL;
noteInfo *currentNoteDisplayQueue = NULL;

noteInfo *headNoteFileQueue = NULL;
noteInfo *tailNoteFileQueue = NULL;
noteInfo *currentNoteFileQueue = NULL;

bool notesFinished = false;

pthread_t noteAdder;
pthread_t queuePopper;
static pthread_attr_t attr;
pthread_mutex_t displayQueueMutex = PTHREAD_MUTEX_INITIALIZER;

static void DisplayModel_readDisplay();
static void *DisplayModel_readNoteToDisplayQueue(void *arg);
static void *DisplayModel_popQueue(void *arg);
static void DisplayModel_addSpacers(int numSpacers);

static void DisplayModel_readDisplay() // testing function to read what's in display queue
{
        noteInfo *it = tailNoteDisplayQueue;
        int i = 0;
        printf("===============================\n\n");
        while (it != NULL)
        {
            printf("Reading    %d     here   %d\n", it->note, i++);
            it = it->nextNote;
        }
        printf("===============================\n\n");
}

// Thread function to retrieve the next note to be played, queue it for display, add empty notes until next note, then sleep for time in ms until next note
static void *DisplayModel_readNoteToDisplayQueue(void *arg)
{
    while (headNoteFileQueue != NULL)
    {
        DisplayQueue_lockDisplayQueueMutex();
        {
            NoteQueue_addNote(NoteQueue_popNote(&headNoteFileQueue, &tailNoteFileQueue, &currentNoteFileQueue), &headNoteDisplayQueue, &tailNoteDisplayQueue);
            DisplayModel_addSpacers((currentNoteFileQueue->timeToNextNote) / FRAME_RATE);
        }
        DisplayQueue_unlockDisplayQueueMutex();
        Utils_sleepForMs(currentNoteFileQueue->timeToNextNote);
    }
    notesFinished = true;
}

// Thread function to pop the display queue at the rate defined by FRAME_RATE
static void *DisplayModel_popQueue(void *arg)
{
    while (headNoteDisplayQueue != NULL)
    {
        Utils_sleepForMs(FRAME_RATE); // questionable
        DisplayQueue_lockDisplayQueueMutex();
        {
            DisplayModel_readDisplay();
            NoteQueue_popNote(&headNoteDisplayQueue, &tailNoteDisplayQueue, &currentNoteDisplayQueue);
            NoteQueue_freeNote(currentNoteDisplayQueue); //may want to save note's info before freeing
        }
        DisplayQueue_unlockDisplayQueueMutex();
    }
}

// Add empty notes to display queue to introduce delay between notes
static void DisplayModel_addSpacers(int numSpacers)
{
    for (int i = 0; i < numSpacers; i++)
    {
        noteInfo *newNote = malloc(sizeof(*newNote)); // WRITE INIT FUNCTION?
        newNote->note = NO_NOTE_CODE;
        newNote->timeToNextNote = 0;
        NoteQueue_addNote(newNote, &headNoteDisplayQueue, &tailNoteDisplayQueue);
        newNote = NULL;
    }
}

void DisplayModel_startDisplayModel()
{
    int timeToFirstNote = NoteQueue_loadNotesFromFile(&headNoteFileQueue, &tailNoteFileQueue);
    pthread_attr_init(&attr);
    DisplayModel_addSpacers(timeToFirstNote/FRAME_RATE + NUM_SPACERS_FOR_DISPLAY_SIZE); //Should potentially be ceiling function instead of int division
    pthread_create(&noteAdder, &attr, DisplayModel_readNoteToDisplayQueue, NULL);
    pthread_create(&queuePopper, &attr, DisplayModel_popQueue, NULL);
}

void DisplayModel_stopDisplayModel()
{
    pthread_join(noteAdder, NULL);
    pthread_join(queuePopper, NULL);
    NoteQueue_deleteNotes(&headNoteFileQueue, &tailNoteFileQueue, &currentNoteDisplayQueue); // safety
    DisplayModel_readDisplay();
    NoteQueue_deleteNotes(&headNoteDisplayQueue, &tailNoteDisplayQueue, &currentNoteDisplayQueue); // safety
    NoteQueue_freeNote(currentNoteDisplayQueue);
    NoteQueue_freeNote(currentNoteFileQueue);
}

noteInfo *DisplayQueue_getHeadNoteDisplayQueue()
{
    return headNoteDisplayQueue;
}

void DisplayQueue_lockDisplayQueueMutex()
{
    pthread_mutex_lock(&displayQueueMutex);
}

void DisplayQueue_unlockDisplayQueueMutex()
{
    pthread_mutex_unlock(&displayQueueMutex);
}