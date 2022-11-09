//This module runs threads for managing the display model to be read for display

#ifndef DISPLAY_MODEL
#define DISPLAY_MODEL

#define FRAME_RATE 300 // needs to be experimentally defined
#define NUM_SPACERS_FOR_DISPLAY_SIZE 15 //dependent on physical display
// TODO: frame rate should be decoupled from adding spacer

// Initialize display model parameters and start display model threads
void DisplayModel_startDisplayModel();

// Get a pointer to the head of the display queue, which is the active note
// Display view should read from head pointer and traversing backwards due to display queue being potentially bigger than physical screen
noteInfo *getheadNoteDisplayQueue();

#endif