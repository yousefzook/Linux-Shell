#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//make copy of a string
char * copyString(char* orig){

    if(orig == NULL)
        return NULL;

    char* copy = NULL;
    char* ptr = orig;
    int size = 0;
    while(*ptr++ != '\0')
        size++;


    copy = (char*) malloc((size+1)*sizeof(char));
    copy[size] = '\0';

    ptr = &orig[0];
    int index = 0;
    while(*ptr != '\0')
        copy[index++] = *ptr++;

    return copy;
}

// concatenate two strings
char * catString(char* first, char* second){

    if(first == NULL)
        return copyString(second);
    if(second == NULL)
        return copyString(first);

    char* result = NULL;
    char* ptr1 = first;
    char* ptr2 = second;
    int size1 = 0;
    int size2 = 0;
    while(*ptr1++ != '\0')
        size1++;
    while(*ptr2++ != '\0')
        size2++;

    result = (char*) malloc((size1+size2+1)*sizeof(char));
    result[size1+size2] = '\0';
    ptr1 = &first[0];
    ptr2 = &second[0];
    int index = 0;
    while(*ptr1 != '\0')
        result[index++] = *ptr1++;
    while(*ptr2 != '\0')
        result[index++] = *ptr2++;

    return result;
}

// trim a string
char * trimString(char str[]){

    char *ptr = str;
    char *result = NULL;
    int size = 0;
    while(ptr[size] >= ' ' && ptr[size] <= '~'){
        size++;
    }

    result = (char*) malloc((size+1) * sizeof(char));
    result[size] = '\0';
    int index = 0;
    while(index < size){
        result[index] = ptr[index];
        index++;
    }

    return result;
}

// get number of character's existence in a string
int getNumOfChar(char *str, char c){
    int result = 0;
    int i = 0 ;
    while(str[i] != NULL){
        if(str[i] == c)
            result++;
        i++;
    }
    return result;
}

// return index of first char = x in the str or -1 if not found
int getIndexOfChar(char* str, char c){
    int i = 0;
    while(str[i] != NULL){
        if(str[i] == c)
            return i;
        i++;
    }
    return -1;
}

// get sub-string
char * subString(char* str, int from, int to){
    char* result = (char*) malloc((to-from+1)*sizeof(char));
    result[to-from] = '\0';
    int i = from;
    int ind=0;
    for(int i = from; i < to; i++){
        result[ind] = str[i];
        ind++;
    }
    return result;
}

// replace a sub string from orig with mod<modification>
char * replaceSubString(char* orig, char* mod, int from, int to){
    char* result;
    int i = 0;
    int size = 0;
    while(orig[i] != NULL){
        i++;
        size++;
    }
    size = size - (to - from);
    i = 0;
    int modSize = 0;
    while(mod[i] != NULL){
        i++;
        modSize++;
        size++;
    }
    result = (char*) malloc((size+1)*sizeof(char));
    result[size] = '\0';
    int c = 0;
    for(i = 0; i < from; i++){
        result[c] = orig[i];
        c++;
    }
    for(i = 0; i < modSize; i++){
        result[c] = mod[i];
        c++;
    }
    for(i = to; orig[i] != NULL; i++){
        result[c] = orig[i];
        c++;
    }
    return result;
}

// remove non-alphabet and non-digit characters from string keeping only '/'
char * stringToPath(char* str){
    char* result;
    int size = 0;
    int i = 0;
    while(str[i] != NULL){
        if(!(str[i] < 48 || (str[i] > 57 && str[i] < 64) ||
             (str[i] > 90 && str[i] < 97) || str[i] > 122) || str[i] == '/'){
            size++;
        }
        i++;
    }
    result = (char*) malloc((size+1)*sizeof(char));
    result[size] = '\0';
    i = 0;
    int j = 0;
    while(str[i] != NULL){
        if(!(str[i] < 48 || (str[i] > 57 && str[i] < 64) ||
             (str[i] > 90 && str[i] < 97) || str[i] > 122) || str[i] == '/'){
            result[j] = str[i];
            j++;
        }
        i++;
    }
    return result;
}

// return size of a string
int getStringSize(char* str){
    int size = 0;
    while(str[size] != NULL)
        size++;
    return size;
}
