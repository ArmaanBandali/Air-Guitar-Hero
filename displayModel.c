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
pthread_t spaceAdder;
pthread_attr_t attr;
pthread_mutex_t displayQueueMutex = PTHREAD_MUTEX_INITIALIZER;

static void DisplayModel_readDisplay();
static void *DisplayModel_readNoteToDisplayQueue(void *arg);
static void *DisplayModel_addSpacerNote(void *arg);
static void DisplayModel_initializeWithSpacers(int numSpacers);
static void DisplayModel_stopDisplayModel();

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

// Thread function to retrieve the next note to be played and queue it for display, then sleep for time in ms until next note
static void *DisplayModel_readNoteToDisplayQueue(void *arg)
{
    while (headNoteFileQueue != NULL)
    {
        pthread_mutex_lock(&displayQueueMutex);
        {
            NoteQueue_addNote(NoteQueue_popNote(&headNoteFileQueue, &tailNoteFileQueue, &currentNoteFileQueue), &headNoteDisplayQueue, &tailNoteDisplayQueue);
        }
        pthread_mutex_unlock(&displayQueueMutex);
        Utils_sleepForMs(currentNoteFileQueue->timeToNextNote);
    }
    notesFinished = true;
}

// Thread function to add spaces (empty notes) between notes at the rate defined by FRAME_RATE
static void *DisplayModel_addSpacerNote(void *arg)
{
    long long startTimeSpacer = Utils_getTimeInMs(); //should use sleep to prevent busy wait
    while (headNoteDisplayQueue != NULL)
    {
        if (Utils_getTimeInMs() - startTimeSpacer > FRAME_RATE)
        {
            startTimeSpacer = Utils_getTimeInMs();
            pthread_mutex_lock(&displayQueueMutex);
            {
                DisplayModel_readDisplay();
                if (!notesFinished)
                {
                    noteInfo *newNote = malloc(sizeof(*newNote));
                    newNote->note = NO_NOTE_CODE;
                    newNote->timeToNextNote = 0;
                    NoteQueue_addNote(newNote, &headNoteDisplayQueue, &tailNoteDisplayQueue);
                    newNote = NULL;
                }
                NoteQueue_popNote(&headNoteDisplayQueue, &tailNoteDisplayQueue, &currentNoteDisplayQueue); //might want to decouple this
                NoteQueue_freeNote(currentNoteDisplayQueue); //may want to save note's info before freeing
            }
            pthread_mutex_unlock(&displayQueueMutex);
        }
    }
}

// Add empty notes to display queue to introduce delay between song start and first note
static void DisplayModel_initializeWithSpacers(int numSpacers)
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
    DisplayModel_initializeWithSpacers(timeToFirstNote/FRAME_RATE + NUM_SPACERS_FOR_DISPLAY_SIZE); //Should potentially be ceiling function instead of int division
    pthread_create(&noteAdder, &attr, DisplayModel_readNoteToDisplayQueue, NULL);
    pthread_create(&spaceAdder, &attr, DisplayModel_addSpacerNote, NULL);
    pthread_join(noteAdder, NULL);
    pthread_join(spaceAdder, NULL);
    DisplayModel_stopDisplayModel();
}

//Clean up memory
static void DisplayModel_stopDisplayModel()
{
    NoteQueue_deleteNotes(&headNoteFileQueue, &tailNoteFileQueue, &currentNoteDisplayQueue); // safety
    DisplayModel_readDisplay();
    NoteQueue_deleteNotes(&headNoteDisplayQueue, &tailNoteDisplayQueue, &currentNoteDisplayQueue); // safety
    NoteQueue_freeNote(currentNoteDisplayQueue);
    NoteQueue_freeNote(currentNoteFileQueue);
}

noteInfo *getheadNoteDisplayQueue()
{
    return headNoteDisplayQueue;
}