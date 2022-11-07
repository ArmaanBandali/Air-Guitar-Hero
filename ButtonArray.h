//This package creates a simple interface to use the buttons as input for the Air-Guitar-Hero project

#ifndef BUTTONARRAY_H
#define BUTTONARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


//This function initializes the buttons as gpio and sets them to input
void initializeButtons();

//user must pass in an array of 5 button values to be edited to contain the values of the buttons
void getButtonValues(int *values);

#endif