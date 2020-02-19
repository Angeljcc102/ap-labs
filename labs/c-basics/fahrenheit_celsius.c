#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int agrc, char **args)
{
    int fahr = atoi(args[1]);

    if(agrc == 2){
      printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
    if(agrc == 4){
      for (fahr = atoi(args[1]); fahr <= atoi(args[2]); fahr = fahr + atoi(args[3]))
      printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }

    return 0;
}
