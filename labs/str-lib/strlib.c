#include <stdio.h>
#include <stdlib.h>


int mystrlen(char *str){
    int len_str = 0;
    while(*str != '\0'){
        len_str++;
        str++;
    }
    return len_str;
}

char *mystradd(char *origin, char *addition){
    int len_origin = mystrlen(origin),
        len_addition = mystrlen(addition),
        len_new = len_origin+len_addition;
    char* new_string = (char *)malloc(len_new+1);
    for(int i = 0; i < len_origin; i++){
        new_string[i] = origin[i];
    }
    for(int j = len_origin; j < len_new; j++){
        new_string[j] = addition[j-len_origin];
    }
    return new_string;
}

int mystrfind(char *origin, char *substr){
    int len_origin = mystrlen(origin),
        len_substr = mystrlen(substr);
    for(int i = 0; i <= len_origin-len_substr; i++){
        int j = 0;
        for(j; j < len_substr; j++){
            if(origin[i+j] != substr[j]){
                break;
            }
        }
        if(j == len_substr){
            return 1;
        }
    }
    return 0;
}
