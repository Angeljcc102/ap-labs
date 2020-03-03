#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static char *month_name(int *pmonth) {
	static char *name[] = {"Illegal month","Jan","Feb","Mar","Apr","May",
			"Jun","Jul","Aug","Sept","Oct","Nov","Dec"};
	return (*pmonth < 1 || *pmonth > 12) ? name[0] : name[*pmonth];
}

void month_day(int year, int yearday, int *pmonth, int *pday){
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	if((leap == 0 && yearday > 365) || (leap == 1 && yearday > 366) || year < 0){
		printf("%s\n", "Invalid year or yearday");
	}
	else{
		int i = 1;
		pday = &yearday;
		for(i; *pday > daytab[leap][i]; i++){
			*pday -= daytab[leap][i];
		}
		pmonth = &i;
		printf("%s %i, %i\n", month_name(pmonth), *pday, year);
	}
} 

int main(int agrc, char **args) {
	if(args[1] == NULL || args[2] == NULL){
		printf("%s\n", "No year or month written");
	}
	else{
   		int year = atoi(args[1]),yearday = atoi(args[2]),*pmonth, *pday;
		month_day(year, yearday, pmonth, pday);
	}
    return 0;
}
