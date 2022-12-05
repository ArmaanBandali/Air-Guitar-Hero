//This module implements an abstract data type queue for holding notes

#ifndef NOTE_HANDLER
#define NOTE_HANDLER

#define NO_NOTE_CODE 0b0000
#define GREEN_NOTE_CODE 0b10000 // could maybe just do bit shifting
#define RED_NOTE_CODE 0b01000
#define YELLOW_NOTE_CODE 0b00100
#define BLUE_NOTE_CODE 0b00010
#define ORANGE_NOTE_CODE 0b00001

// Queue node containing information for each note and forward and backward node pointers
typedef struct NoteInfo
{
    int note;
    int timeToNextNote;
    struct NoteInfo *nextNote;
    struct NoteInfo *previousNote;
} noteInfo;

// Loads notes from a note file to the queue defined by head and tail pointers and return the time in ms to the first note
// Preconditions: -first line in notes file specifies time in ms to first note
//                -all lines beginning with '$' are comments
//                -last note is terminated with '$'
//                -each valid note line has a note code (any combination of characters 'G', 'R', 'Y', 'B', 'O') and the time in ms until the next note
//                -max line length is 1024 characters
int NoteQueue_loadNotesFromFile(char* songNoteFile, noteInfo **headNote, noteInfo **tailNote);

// Delete all notes from the queue defined by head and tail pointers
void NoteQueue_deleteNotes(noteInfo **headNote, noteInfo **tailNote, noteInfo **currentNote);

// Free note memory
void NoteQueue_freeNote(noteInfo *currentNote);

// Pop note from the queue defined by head and tail pointers
//Returns the popped note and also stores in curentNote
noteInfo *NoteQueue_popNote(noteInfo **headNote, noteInfo **tailNote, noteInfo **currentNote);

// Add a note to the queue defined by head and tail pointers
void NoteQueue_addNote(noteInfo *note, noteInfo **headNote, noteInfo **tailNote);

#endif