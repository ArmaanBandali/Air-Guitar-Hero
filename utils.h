// Module for general system and platform utilities

#ifndef UTILS
#define UTILS

// Dr. Brian's function
// Program sleeps for specified time in ms
void Utils_sleepForMs(long long delayInMs);

// Dr. Brian's function
// Return current Unix time in ms
long long Utils_getTimeInMs(void);

// Dr. Brian's code to run linux command in terminal
void Utils_runCommand(char *command);

// Write arg to file specified by path
void Utils_writeToFile(char *path, char *arg);

// Read a single int value from file 
// For gpio values like buttons
int Utils_readIntFromFile(char *button);

// helper function to concat two strings
// from https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
char *Utils_concat(const char *s1, const char *s2);

#endif