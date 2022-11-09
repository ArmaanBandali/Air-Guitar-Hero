//This package creates a simple interface to use the buttons as input for the Air-Guitar-Hero project

#ifndef BUTTONARRAY_H
#define BUTTONARRAY_H

//This function initializes the buttons as gpio and sets them to input
void ButtonArray_initializeButtons();

//user must pass in an array of 5 button values to be edited to contain the values of the buttons
void ButtonArray_getButtonValues(int *values);

void ButtonArray_test();

#endif