#include "commands.h"
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

char* currentPath;

char* getCurrentPath(){
    return copyString(currentPath);
}

char* getUserDir(){ // returns the home value
    return lookup_variable("HOME");
}


void cd( const char* path ){

    int result;
    if(path == NULL){
        result = chdir(getUserDir());
    }
    char* newPath;
    if(strcmp(path,"../") == 0){ // go to previous folder
        int sizeOfPath = getStringSize(currentPath);
        int i = sizeOfPath-1;
        while(currentPath[i] != '/') //loop tell reach last '/' character in path
            i--;
        if(i == 0)
            i++;
        newPath = subString(currentPath,0,i); // remove the last folder from path
    }else
        newPath = stringToPath(path); // stringToPath exists in stringOperations.c <to remove unwanted characters>
    result = chdir(newPath);
    if(result != 0)
        perror("Error in cd: ");
    else
        currentPath = newPath;
}

void echo( const char* message ){

    int cnt = 0;
    int sizeOfMess = 0;
    while(message[sizeOfMess] != NULL)// to get size of message
        sizeOfMess++;
    while(message[cnt] != NULL && message[cnt] != '$'){
        cnt++;
    }
    if(message[cnt] == NULL && message[0] == '"'){ // contains '"' character
        // removing '"' character
        char* result;
        int size = 0;
        int i = 0;
        while(message[i] != NULL){
            if(message[i] != '"'){
                size++;
            }
            i++;
        }
        result = (char*) malloc((size+1)*sizeof(char));
        result[size] = '\0';
        i = 0;
        int j = 0;
        while(message[i] != NULL){
            if(message[i] != '"'){
                result[j] = message[i];
                j++;
            }
            i++;
        }
        printf("%s\n",result); //result = message without '"' character
        return;
    }

    if(message[cnt] == NULL)// has no variables or '"' characters, then print it
        printf("%s\n",message);

    else{ //contain $ sign
        cnt++; //counter
        int vL = cnt; // variable location
        int found = 0;
        while(message[cnt] != NULL){
            if(message[cnt] < 48 || (message[cnt] > 57 && message[cnt] < 64)
               || (message[cnt] > 90 && message[cnt] < 97) || message[cnt] > 122){
                found = -1; // that means that after '$' sign we reached a non-alphabitical and non-digit char so stop
                break;
            }
            cnt++;
            char* subMess = subString(message, vL, cnt); // to lookup if it is a variable or not
            char* value = lookup_variable(subMess);
            if(value != NULL){// this is a variable
                char* result = replaceSubString(message,value,(vL-1),cnt); // remove $<var> with its value
                printf("%s\n",result); // print the result
                found = 1;// that make that replacement of the variable is successful
                break;
            }
        }
        if(found == 0 && cnt != vL){ //there is no variable of this name
            printf("%s\n",subString(message,0,vL-1));
        }else if(found == 0){ //this is a '$' sign with no variable after it, so print it
            printf("%s\n",subString(message,0,vL));
        }else if (found == -1){ //remove $ sign and the alphabets after it then print
            printf("%s%s\n",subString(message,0,vL-1),subString(message,cnt,sizeOfMess));
        }

    }

}

int myExport(char* expression) { // to handle export cases


    if(expression[0] == '$'){ // as export $x=5
//        printf("Error in Export: Not Valid Identifier!\n");
        return 0;
    }

    int i = 0 ;
    char* key;
    char* value;
    while(expression[i] != NULL && expression[i] != '=')
        i++;

    if(expression[i] == NULL) // like export x
        return 0;

    // getting key and value separately
    key = (char*) malloc((i+1)*sizeof(char));
    key[i] = '\0';
    i = 0;
    while(expression[i] != '='){
        key[i] = expression[i];
        i++;
    }
    i++;
    int beg = i;
    while(expression[i] != NULL)
        i++;

    int sizeOfValue = i-beg;
    value = (char*)malloc((i-beg+1)*sizeof(char*));
    value[i-beg] = '\0';
    int j = 0;
    i = beg;
    int qutsNum = 0;     // number of removed " character
    while(expression[i] != NULL){
        if(expression[i]=='"'){ // removing " character
            i++;
            qutsNum++;
            continue;
        }
        value[j] = expression[i];
        i++;
        j++;
    }
    value = subString(value,0, sizeOfValue-qutsNum);
    // then set the variable with its value
    set_variable(key,value);
    return 1;

}
