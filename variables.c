#include "variables.h"
#include <stdlib.h>

// all variables in enviroment
char** envVar;

void setEnvVar(char* envp[]){
    envVar = envp;
}



const char* lookup_variable( const char* key )
{
    int i = 0;
    while (envVar[i] != NULL){
        char* var = envVar[i];
        int j = 0;
        while(var[j] == key[j])
            j++;
        if(var[j] == '=' && key[j] == NULL){ // this mean that the variable is exist
            char *result; // the vlaue of the key
            j++;
            int beg = j;
            while(var[j] != NULL)
                j++;
            result = (char*) malloc((j-beg+1)*sizeof(char));
            result[j-beg] = '\0';
            int c = 0;
            while(beg < j){
                result[c] = var[beg];
                c++;
                beg++;
            }
            return result;
        }
        i++;
    }
    return NULL; // if not exist return null
}

void set_variable( const char* key , const char* value )
{
    char** temp;
    int size = 0;
    int iter = 0;
    char* var = catString(key,"=");
    var = catString(var,value); // for example 'x=5' where key = x and value = 5

    while(envVar[size] != NULL) // to get size of envVar
        size++;

    while(envVar[iter] != NULL){
        int iter2 = 0;
        while(envVar[iter][iter2] != NULL && key[iter2] != NULL && envVar[iter][iter2] == key[iter2])
            // if the variable-envVar[iter]- key is still equal to key
            iter2++;

        if(key[iter2] == NULL && envVar[iter][iter2] != NULL && envVar[iter][iter2] == '='){
            // the key is exist at envVar[iter]
            break;
        }
        iter++;
    }

    if(size != iter){ // this key is exit in the variables
        envVar[iter] = var; // replace the old by the new
        return;
    }
    // else, make new variable and set it in envVar
    temp = (char**) malloc((size+2)*sizeof(char*));
    temp[size+1] = NULL;
    int i = 0;
    for(i = 0; i < size; i++)
        temp[i] = envVar[i];
    unsetenv(key);
    setenv(key,value,1);
    temp[size] = var;
    envVar = temp;
}

void print_all_variables( void )
{
    // you should implement this function
    int i;
    for (i = 0; envVar[i] != NULL; i++){
        printf("%s\n", envVar[i]);
    }
}
