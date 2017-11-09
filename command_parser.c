#include "command_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// handler for child termination signal
void handler(int sig){
    open_log_file();
    FILE *lF = get_log_file();
    fprintf(lF,"child process was terminated\n");
    close_log_file();
}

// function to change the variables in argument list to their values
int chArgVar(char* list[]){
    int i = 0;
    int result = 0;
    while(list[i] != NULL){
        if(getNumOfChar(list[i],'$') > 0){
            int ind = getIndexOfChar(list[i],'$');
            int j = ind+1;
            while(list[i][j] != NULL){
                char* value = lookup_variable(subString(list[i], ind+1, j+1));
                if(value != NULL){
                    list[i] = replaceSubString(list[i], value, ind, j+1);
                    i--;
                    result = 1;
                    break;
                }
                j++;
            }
        }
        i++;
    }
    return result;
}

// combile the elements of a list to make one string called command
char* getCommandFromList(char* list[]){
    int i = 0;
    char* command = NULL;
    while(list[i] != NULL){
        command = catString(command, list[i]);
        i++;
    }

    return command;
}

void parse_command( const char* command){


    // you should implement this function
    char* commandCopy = copyString(command);
    char* token = strtok(commandCopy, " ");
    char* inst = token; // first word in the line

    if(token == NULL || token[0] == '#')
        return;
    if(strcmp(token,"exit")==0){
        exit(0);
    }
    if(strcmp(token,"history") == 0){
        printHistoryFile();
        return;
    }

    // cheking for export and variable setting
    if(strcmp(token,"export") == 0){

        if(getNumOfChar(command,'"') > 0){
            myExport(trimString(subString(command,7,getStringSize(command))));
            return;
        }

        token = strtok(NULL, " ");
        if(token == NULL)
            return;

        myExport(token);// function myExport in commands.c
        return;
    }

    else if(getNumOfChar(command,'"') > 0){
        myExport(command);
        return;
    }

    // checking for change directory
    if(strcmp(token,"cd") == 0){
        token = strtok(NULL, " ");
        if(token == NULL || strcasecmp(token,"~") == 0){
            cd(getUserDir());
            return;
        }
        char* tokenCopy = copyString(token);
        char* tokenizer = strtok(tokenCopy,"/");// word before '/'
        if(tokenizer[0] == '~'){ // this like form of "~/etc"
            const char* temp;
            temp = getUserDir();
            temp = catString(temp,"/");
            tokenizer = strtok(NULL," ");
            temp = catString(temp,tokenizer);// in form "/home/userName/etc"
            cd(temp);
            return;
        }
        cd(token);
        return;
    }

    // checking for echo
    if(strcmp(token,"echo") == 0){
        token = strtok(NULL, " ");
        if(token == NULL)
            return;
        if(token[0] == '"'){
            echo(subString(command,5,getStringSize(command)));
            return;
        }
        echo(token);
        return;
    }




    // if not exit, echo, export, cd or history
    char *instDir="";
    char *parmList[100];

    parmList[0] = inst;
    token = strtok(command, " ");
    int i = 1;
    while(token){
        token = strtok(NULL, " ");
        parmList[i] = token;
        i++;
    }

    if(parmList[1]==NULL){
        int isExpression = myExport(inst);
        if(isExpression)
            return;
    }

    // check if there is a variables in the argument list
    int result = chArgVar(parmList);
    if(result){// that mean that there were variables in the list
        char* executableComm = getCommandFromList(parmList);
        parse_command(executableComm);
        return;
    }

    //fork the processes
    signal(SIGCHLD, handler);
    pid_t id = fork();
    if(id == 0){ // children process

        open_log_file();
        FILE *lF = get_log_file();
        fprintf(lF,"child process created for instruction: %s\n",inst);
        close_log_file();

        int chldExitCode = 0;
        if(strcmp(parmList[i-2], "&") == 0) // check for & character
            parmList[i-2] = NULL;

        char* enviromentVarPATH = getenv("PATH"); // PATH value
        char* enviromentVar = copyString(enviromentVarPATH);
        char* vars[100]; // array to contatin all PATH's variables
        char* tokeniz = strtok(enviromentVar,":"); // tokeniz = first value in PATH

        vars[0] = tokeniz;
        instDir = vars[0];
        if(inst[0] != '/'){
            instDir = catString(instDir,"/");
            instDir = catString(instDir,inst); // in form -for example- /bin/ls
        }
        else
            instDir = copyString(inst);

        chldExitCode = execv(instDir,parmList);
        char* PATH = lookup_variable("PATH");
        int sizeOfPATH = getNumOfChar(PATH,':') + 1; // number of variables in PATH
        int pathCnt = 1;
        while(pathCnt < sizeOfPATH){ // looping tell execution or end of vaiables
            tokeniz = strtok(NULL,":");
            vars[pathCnt] = tokeniz;
            instDir = vars[pathCnt];
            instDir = catString(instDir,"/");
            instDir = catString(instDir,parmList[0]);
            chldExitCode = execv(instDir,parmList);
            pathCnt++;
        }
        if(chldExitCode != 0) // if not exist
            perror(ANSI_COLOR_RED  "Error in execution:"  ANSI_COLOR_RESET);


        exit(-1);
    }else{ // parent process
        if(strcmp(parmList[i-2], "&") != 0) // if '&' exist, then don't wait
            wait();

        else
            parmList[i-2] = NULL; // to remove '&' from list
    }
}
