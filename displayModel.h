//This module runs threads for managing the display model to be read for display

#ifndef DISPLAY_MODEL
#define DISPLAY_MODEL

#include "noteQueue.h"

#define FRAME_RATE 300 // needs to be experimentally defined or defined per song
#define NUM_SPACERS_FOR_DISPLAY_SIZE 15 //dependent on physical display

// Initialize display model parameters and start display model threads
void DisplayModel_startDisplayModel();

// Clean up memory
void DisplayModel_stopDisplayModel();

// Get a pointer to the head of the display queue, which is the active note
// Display view should read from head pointer and traversing backwards due to display queue being potentially bigger than physical screen
noteInfo *DisplayQueue_getHeadNoteDisplayQueue();

// Lock display queue
void DisplayQueue_lockDisplayQueueMutex();

//UNlock display queue
void DisplayQueue_unlockDisplayQueueMutex();

#endif