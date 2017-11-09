#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <signal.h>

// these is for some colors in cosole
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum{ false = 0 , true = 1 } bool ;

void start(bool read_from_file);
void shell_loop(bool input_from_file);
FILE* commandFile; // batch file
char *filePath;

// this function to make list of commands from batch file dynamic in size
char** doubleList(char* list[], int size){
    char* result[size*2];
    int i = 0;
    for(i = 0; i < size; i++)
        result[i] = list[i];
    return result;
}

// this is for ctrl+c signal <but it is corrupted in batch mode!>
//static volatile int keepRunning = 1;
//void intHandler(int dummy) {
//    keepRunning = 0;
//}

int main(int argc, char *argv[], char * envp[]){

    setEnvVar(envp); // send enviroment variables

    // this is for ctrl+c signal <but it is corrupted in batch mode!>
//    signal(SIGINT, intHandler);
//    while(keepRunning){

        if( argc > 1 ){
            filePath = argv[1];
            start(true);
        }
        else{
            start(false);
        }
//    }
    return 0;
}

void start(bool read_from_file){

    cd(getUserDir()); // let shell starts from /home/<userName>

    if(read_from_file){
        // file processing functions
        open_commands_batch_file(filePath);
        commandFile = get_commands_batch_file();
        shell_loop(true);
        close_commands_batch_file();
    }
    else{
        shell_loop(false);
    }
}

void shell_loop(bool input_from_file){
    bool from_file = input_from_file;
    while(true){

        if(from_file){

            char commandLine[512]; // each line in the file
            char* commandList[100];// will contain all commands to execute
            int i = 0;
            int listSize = 100;
            while(true)
            {
                fgets(commandLine,512,commandFile);
                if(feof(commandFile)){
                    break;
                }
                char* trimmedCom = trimString(commandLine);// trim command line
                commandList[i] = trimmedCom;
                i++;
                if(i == listSize){
                    commandList[0] = doubleList(commandList, listSize);
                    listSize *= 2;
                }
            }

            // execute teh batch
            int j = 0 ;
            for(j = 0; j < i; j++){
                printf(ANSI_COLOR_GREEN "%s"  ANSI_COLOR_RESET "\n",commandList[j]);

                parse_command(commandList[j]);

                open_history_file();
                FILE* hF = get_history_file();
                fprintf(hF,commandList[j]);
                fprintf(hF,"\n");
                close_history_file();
            }
            break; // stop the shell

        }
        else{
            //read next instruction from console
            char commandLine[] = "";
            printf(ANSI_COLOR_CYAN "Shell:" ANSI_COLOR_GREEN "%s" ANSI_COLOR_CYAN"> " ANSI_COLOR_RESET,getCurrentPath());
            scanf("%[^\n]", &commandLine);//scan all line include spaces
            getchar();

            parse_command(commandLine);

            // edit history file
            open_history_file();
            FILE* hF = get_history_file();
            fprintf(hF,commandLine);
            fprintf(hF,"\n");
            close_history_file();
        }

    }
}
