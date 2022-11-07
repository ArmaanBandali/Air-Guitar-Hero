
#include "ButtonArray.h"
#define BUTTON_1 "/sys/class/gpio/gpio70"
#define BUTTON_2 "/sys/class/gpio/gpio71"
#define BUTTON_3 "/sys/class/gpio/gpio78"
#define BUTTON_4 "/sys/class/gpio/gpio79"
#define BUTTON_5 "/sys/class/gpio/gpio81"

#define VALUE_PATH "/value"
#define DIRECTION_PATH "/direction"
#define ACTIVE_LOW_PATH "/active_low"
#define BUTTON_ARRAY_SIZE 5

char* ButtonArray[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5};

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

//helper function to concat two strings
//from https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
static char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void getButtonValues(int *values){
    for(int i = 0; i< BUTTON_ARRAY_SIZE; i++){
        values[i] = readFromFile(concat(ButtonArray[i], VALUE_PATH));
    }
}
void initializeButtons(){
    //set each config
    runCommand("config-pin P8.34 gpio");
    runCommand("config-pin P8.38 gpio");
    runCommand("config-pin P8.37 gpio");
    runCommand("config-pin P8.45 gpio");
    runCommand("config-pin P8.46 gpio");
    for(int i=0; i < BUTTON_ARRAY_SIZE; i++){
        char* directionPath = concat(ButtonArray[i], DIRECTION_PATH);
        writeToFile(directionPath, "in");
        char* active_low= concat(ButtonArray[i], ACTIVE_LOW_PATH);
        writeToFile(active_low, "0");

    }
}


void test(){
    while(true){
        int values[BUTTON_ARRAY_SIZE];
        getButtonValues(values);
        for(int i=0; i< BUTTON_ARRAY_SIZE; i++){
            printf("Button %i: %i\n", i, values[i]);
        }
        sleepForMS(100);
    }
}

int main(){
    initializeButtons();
    test();
    return 0;
}
