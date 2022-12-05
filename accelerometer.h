// Interface to read values from the accelerometer
// Determines if the right hand is being strum or not
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#define X_FILE_VOLTAGE2 "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define ACCEL_THRESHOLD_LOW 1900
#define ACCEL_THRESHOLD_HIGH 2200


// sleep func
void sleepForMs(long long delayInMs);

// init() must be called before any other functions,
// cleanup() must be called last to stop playback threads and free memory.
void accelerometer_init(void);
void accelerometer_cleanup(void);


// Read file
int getXReading();

// accel mutex
void accelerometer_unlockMutex();
void accelerometer_lockMutex();


#endif