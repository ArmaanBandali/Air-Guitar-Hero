#include "accelerometer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


// Accel threading
void* accelerometerThread(void* arg);
static _Bool stopping = false;
static pthread_t accelThreadId;
static pthread_mutex_t accelMutex = PTHREAD_MUTEX_INITIALIZER;

void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

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
    while(!stopping) {
        x = getXReading();

        if(x < ACCEL_THRESHOLD_LOW || x > ACCEL_THRESHOLD_HIGH){
            printf("Strum:%i \n", x);
        }
        sleepForMs(500);
    }

    return NULL;
}


void accelerometer_unlockMutex()
{
    pthread_mutex_lock(&accelMutex);
}

void accelerometer_lockMutex()
{
    pthread_mutex_unlock(&accelMutex);
}

