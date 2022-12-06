#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "buttonArray.h"
#include "utils.h"

#define BUTTON_1 "/sys/class/gpio/gpio70"
#define BUTTON_2 "/sys/class/gpio/gpio71"
#define BUTTON_3 "/sys/class/gpio/gpio78"
#define BUTTON_4 "/sys/class/gpio/gpio79"
#define BUTTON_5 "/sys/class/gpio/gpio81"

#define VALUE_PATH "/value"
#define DIRECTION_PATH "/direction"
#define ACTIVE_LOW_PATH "/active_low"
#define BUTTON_ARRAY_SIZE 5
#define BUTTON_SLEEP_TIME_MS 3

char* ButtonArray[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5};

static int currentButtonValue = 0;
static bool finishedSampling = false;

static pthread_attr_t attr;
static pthread_t buttonArrayThread;
static pthread_mutex_t buttonArrayMutex = PTHREAD_MUTEX_INITIALIZER;

static void *ButtonArray_startSampling(void *_);

static void *ButtonArray_startSampling(void *_)
{
    while(!finishedSampling)
    {
        int values[BUTTON_ARRAY_SIZE];
        ButtonArray_getButtonValues(values);
        ButtonArray_lockButtonMutex();
        {
            currentButtonValue = ButtonArray_buttonValuesToNote(values);
        }
        ButtonArray_unlockButtonMutex();
        Utils_sleepForMs(BUTTON_SLEEP_TIME_MS);
    }
    return 0;
}


void ButtonArray_start()
{
    ButtonArray_initializeButtons();
    pthread_attr_init(&attr);
    pthread_create(&buttonArrayThread, &attr, ButtonArray_startSampling, NULL);

}

void ButtonArray_stop()
{
    finishedSampling = true;
    pthread_join(buttonArrayThread, NULL);
}

void ButtonArray_getButtonValues(int *values)
{
    for(int i = 0; i< BUTTON_ARRAY_SIZE; i++){
        values[i] = Utils_readIntFromFile(Utils_concat(ButtonArray[i], VALUE_PATH));
    }
}

void ButtonArray_initializeButtons()
{
    //set each config
    Utils_runCommand("config-pin P8.34 gpio");
    Utils_runCommand("config-pin P8.38 gpio");
    Utils_runCommand("config-pin P8.37 gpio");
    Utils_runCommand("config-pin P8.45 gpio");
    Utils_runCommand("config-pin P8.46 gpio");
    for(int i=0; i < BUTTON_ARRAY_SIZE; i++){
        char *directionPath = Utils_concat(ButtonArray[i], DIRECTION_PATH);
        Utils_writeToFile(directionPath, "in");
        char *active_low = Utils_concat(ButtonArray[i], ACTIVE_LOW_PATH);
        Utils_writeToFile(active_low, "0");
        free(active_low);
        free(directionPath);
    }
}

void ButtonArray_test()
{
    while(true){
        int values[BUTTON_ARRAY_SIZE];
        ButtonArray_getButtonValues(values);
        for(int i=0; i< BUTTON_ARRAY_SIZE; i++){
            printf("Button %i: %i\n", i, values[i]);
        }
        ButtonArray_buttonValuesToNote(values);
        Utils_sleepForMs(100);
    }
}

int ButtonArray_buttonValuesToNote(int* values)
{
    int notePlayed = 0;
    for (int i=0; i < BUTTON_ARRAY_SIZE; i++)
    {
        values[i] = values[i] << (BUTTON_ARRAY_SIZE - 1 - i);
        notePlayed = notePlayed | values[i];
        //printf("value%d            note%d\n", values[i], notePlayed);
    }
    return notePlayed;
}

void ButtonArray_lockButtonMutex()
{
    pthread_mutex_lock(&buttonArrayMutex);
}

void ButtonArray_unlockButtonMutex()
{
    pthread_mutex_unlock(&buttonArrayMutex);
}

int ButtonArray_getCurrentButtonValue()
{
    return currentButtonValue;
}