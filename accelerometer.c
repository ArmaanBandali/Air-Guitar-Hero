#include "accelerometer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "utils.h"


// Accel threading
void* accelerometerThread(void* arg);
static _Bool stopping = false;
static pthread_t accelThreadId;
static pthread_mutex_t accelMutex = PTHREAD_MUTEX_INITIALIZER;
static bool strumHit = false;

void accelerometer_init(void)
{
    // Launch accel thread:
	pthread_create(&accelThreadId, NULL, accelerometerThread, NULL);
}

void accelerometer_cleanup(void)
{
    stopping = true;
    pthread_join(accelThreadId, NULL);
}

int getXReading()
{
    // Open file
    FILE *f = fopen(X_FILE_VOLTAGE2, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }

    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);

    return a2dReading;
}

// Thread function to read the accelerometer values
void* accelerometerThread(void* _arg)
{
    int x = 0;
    int reset = 0;
    while(!stopping) {
        x = getXReading();
        if (reset) {
            if (x > ACCEL_THRESHOLD_LOW && x < ACCEL_THRESHOLD_HIGH)
            {
                reset = 0;  
            }
            accelerometer_lockMutex();
            {
                accelerometer_setStrum(false);
            }
            accelerometer_unlockMutex();
        } 
        else {
            if (x < ACCEL_THRESHOLD_LOW || x > ACCEL_THRESHOLD_HIGH)
            {
                reset = 1;
                //printf("Strum \n");
                accelerometer_lockMutex();
                {
                    accelerometer_setStrum(true);
                }
                accelerometer_unlockMutex();
            }
        }
       Utils_sleepForMs(ACCEL_SAMPLING_PERIOD);
    }

    return NULL;
}


void accelerometer_lockMutex()
{
    pthread_mutex_lock(&accelMutex);
}

void accelerometer_unlockMutex()
{
    pthread_mutex_unlock(&accelMutex);
}

void accelerometer_setStrum(_Bool setStrum)
{
    strumHit = setStrum;
}

bool accelerometer_getStrum()
{
    return strumHit;
}
