#include "file_processing.h"
#include <stdio.h>
#include <malloc.h>
#include <pwd.h>



/*
	history file section
*/

char* userDirectory(){
    struct passwd *passwd;
    char* userDir;
    passwd = getpwuid ( getuid());
    userDir = passwd->pw_dir;
    return userDir;
}

FILE *historyFile;

void open_history_file()
{
    char* filePath = catString(userDirectory(),"/historyFile.txt");
    historyFile = fopen(filePath,"a+");
    if(historyFile == NULL){
        perror("No such history file is exist!:");
    }
}

FILE* get_history_file()
{
    return historyFile;
}

void close_history_file()
{
    fclose(historyFile);
}


/*
	log file section
*/
FILE *logFile;
void open_log_file()
{
    char* filePath = catString(userDirectory(),"/logFile.txt");
    logFile = fopen(filePath,"a");
    if(logFile == NULL){
        perror("No such log file is exist!:");
    }
}

FILE* get_log_file()
{
    // you should implement this function
    return logFile;
}

void close_log_file()
{
    // you should implement this function
    fclose(logFile);
}


/*
	CommandsBatch file section
*/

FILE *commandFile;
void open_commands_batch_file(char *path)
{
    commandFile = fopen(path,"r");
    if(commandFile == NULL){
        perror("No such command file is exist!:");
    }
}

FILE* get_commands_batch_file()
{
    return commandFile;
}

void close_commands_batch_file()
{
    // you should implement this function
    fclose(commandFile);
}

// this function to print history file
void printHistoryFile(){
    open_history_file();
    char *line;
    while(!feof(historyFile)){ // this is feof
        line = (char*)malloc(512* sizeof(char));
        fscanf(historyFile,"%[^n]%*c", line);
        printf("\n%s",line);
    }
    close_history_file();
}
