#include <stdio.h>


int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char* argv[]) {

    if(argc < 4 || argc > 4){
        printf("Usage: ./main [origin] [addition] [substring] \n");
        return 0;
    }
    
    int a = 10;

    printf("Initial Lenght      : %d\n",mystrlen(argv[1]));
    printf("New String          : %s\n",mystradd(argv[1],argv[2]));
    printf("SubString was found : %s\n",mystrfind(mystradd(argv[1],argv[2]),argv[3])==1? "yes":"no");

}
