//This package creates a simple interface to use the buttons as input for the Air-Guitar-Hero project


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BUTTON_1_VALUE "/sys/class/gpio/gpio70/value"
#define BUTTON_2_VALUE "/sys/class/gpio/gpio71/value"
#define BUTTON_3_VALUE "/sys/class/gpio/gpio78/value"
#define BUTTON_4_VALUE "/sys/class/gpio/gpio79/value"
#define BUTTON_5_VALUE "/sys/class/gpio/gpio81/value"

//This function initializes the buttons as gpio and sets them to input
void initializeButtons();


int getButtonValue(char* button);