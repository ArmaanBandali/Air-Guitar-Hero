#ifndef DISPLAY_MODEL
#define DISPLAY_MODEL

#define FRAME_RATE 300 // needs to be experimentally defined
#define NUM_SPACERS_FOR_DISPLAY_SIZE 15 //dependent on physical display
// TODO: frame rate should be decoupled from adding spacer

void DisplayModel_startDisplayModel();
void DisplayModel_stopDisplayModel();

void *DisplayModel_addSpacerNote(void *arg);
void DisplayModel_readDisplay(); //only for testing with terminal, should be in display module
void *DisplayModel_readNoteToDisplayQueue(void *arg);
void DisplayModel_initializeWithSpacers(int numSpacers);

#endif