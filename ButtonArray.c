#ifndef BUTTONARRAY_H
#define BUTTONARRAY_H
#include "ButtonArray.h"

static void writeToFile(char* path, char* arg){
    // Use fopen() to open the file for write access.
    FILE *pFile = fopen(path, "w");
    if (pFile == NULL) {
    printf("ERROR: Unable to open file.\n");
    exit(1);
    }
    // Write to data to the file using fprintf():
    fprintf(pFile, "%s", arg);
    // Close the file using fclose():
    fclose(pFile);
}

//Dr. Brian's code to run linux command in terminal 
static void runCommand(char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

static void sleepForMS(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

static int readFromFile(char* button){
      FILE* readFile = fopen(button, "r");
    if(!readFile){
        printf("error: could not open input file\n");
        exit(1);
    }
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, readFile);
    return atoi(buff);
}


void initializeButtons(){
    //set each config
    runCommand("config-pin P8.34 gpio");
    runCommand("config-pin P8.38 gpio");
    runCommand("config-pin P8.37 gpio");
    runCommand("config-pin P8.45 gpio");
    runCommand("config-pin P8.46 gpio");
    writeToFile("/sys/class/gpio/gpio70/direction", "in");
    writeToFile("/sys/class/gpio/gpio71/direction", "in");
    writeToFile("/sys/class/gpio/gpio78/direction", "in");
    writeToFile("/sys/class/gpio/gpio79/direction", "in");
    writeToFile("/sys/class/gpio/gpio81/direction", "in");
    writeToFile("/sys/class/gpio/gpio70/active_low", "0");
    writeToFile("/sys/class/gpio/gpio71/active_low", "0");
    writeToFile("/sys/class/gpio/gpio78/active_low", "0");
    writeToFile("/sys/class/gpio/gpio79/active_low", "0");
    writeToFile("/sys/class/gpio/gpio81/active_low", "0");
}

int getButtonValue(char* button){
    return readFromFile(button);
}

int main(){
    initializeButtons();
    while(true){
        printf("Button 1: %i\n", getButtonValue(BUTTON_1_VALUE));
        printf("Button 2: %i\n", getButtonValue(BUTTON_2_VALUE));
        printf("Button 3: %i\n", getButtonValue(BUTTON_3_VALUE));
        printf("Button 4: %i\n", getButtonValue(BUTTON_4_VALUE));
        printf("Button 5: %i\n\n", getButtonValue(BUTTON_5_VALUE));
        sleepForMS(100);
    }
    return 0;
}
#endif