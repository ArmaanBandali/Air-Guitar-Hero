#ifndef NOTE_HANDLER
#define NOTE_HANDLER

#define NO_NOTE_CODE 0b0000
#define GREEN_NOTE_CODE 0b10000 // could maybe just do bit shifting
#define RED_NOTE_CODE 0b01000
#define YELLOW_NOTE_CODE 0b00100
#define BLUE_NOTE_CODE 0b00010
#define ORANGE_NOTE_CODE 0b00001

typedef struct NoteInfo
{
    int note;
    int timeToNextNote;
    struct NoteInfo *nextNote;
    struct NoteInfo *previousNote;
} noteInfo;

int NoteQueue_loadNotesFromFile(noteInfo **headNote, noteInfo **tailNote);

void NoteQueue_deleteNotes(noteInfo **headNote, noteInfo **tailNote, noteInfo **currentNote);
void NoteQueue_freeNote(noteInfo *currentNote);
noteInfo *NoteQueue_popNote(noteInfo **headNote, noteInfo **tailNote, noteInfo **currentNote);
void NoteQueue_addNote(noteInfo *note, noteInfo **headNote, noteInfo **tailNote);

#endif