#include <stdio.h>

int main() {
	char text[500];
    int c,temp = 0;

	for (int i = 0;(c = getchar()) != EOF && i < 500; i++) {
        text[i] = c;
	if(c == '\n'){
		for(int j = i; j >= temp; j--){
		 	if(text[j] == '\n'){
		 	  }
		 	else{
		 	printf("%c", text[j]);
		    }	
	    }	
	    printf("%c",'\n');
	    temp = i;
		}
    }
	return 0;
}
