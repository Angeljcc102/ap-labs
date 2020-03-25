#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mergeSort(int array[], int first, int last);
void merge(int array[], int first, int middle, int last);

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Usage: ./generic_merge_sort [integer] [integer]\n");
        exit(1);
    }
    int integer_array[argc-1];
    for(int l = 1; l < argc; l++){
        if(argv[l] == "0"){
            integer_array[l-1] = 0;
        }
        else{
            int number = atoi(argv[l]);
            if(number == 0){
                printf("Not valid argument at position %d\n", l);
                exit(1);
            }
            else{
                integer_array[l-1] = number;
            }
        }
    }

    int size_array = sizeof(integer_array)/sizeof(int);

    printf("Original array:");
    for(int i = 0; i < size_array-1; i++){
        printf("%d,",integer_array[i]);
    }
    printf("%d\n",integer_array[size_array-1]);

    mergeSort(integer_array, 0, size_array-1);
    printf("Sorted array:");
    for(int j = 0; j < size_array-1; j++){
        printf("%d,",integer_array[j]);
    }
    printf("%d\n",integer_array[size_array-1]);
}

void mergeSort(int array[], int first, int last){
    if(first < last){
        int middle = (first+last)/2;
        mergeSort(array, first, middle);
        mergeSort(array, middle+1, last);
        merge(array, first, middle, last);
    }
}

void merge(int array[], int first, int middle, int last){
    int left_size = middle-first+1,
        right_size = last-middle;
    int left[left_size],
        right[right_size];
    for(int i = 0; i < left_size; i++){
        left[i] = array[first+i];
    }
    for(int j = 0; j < right_size; j++){
        right[j] = array[middle+j+1];
    }
    int i = 0;
    int j = 0;
    for(int k = first; k <= last; k++){
        if((j >= right_size) || (i < left_size && left[i] <= right[j])){
            array[k] = left[i];
            i++;
        }
        else{
            array[k] = right[j];
            j++;
        }
    }
}