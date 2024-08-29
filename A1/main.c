/* 
 * Jack Porter, Purdue ECE368
 * Assignment 1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    if(argc < 2) {
        printf("Too few arguments supplied\n");
        return 1;
    }

    // read in the change to make
    int val;
    //scanf("%d", &val); //artifact from my misinterpretation of "standard input" (see A1_updated.pdf)
    val = atoi(argv[1]);
    // count down from the maximum number of possible each coin (except pennies) and print
    for (int q=val/25;q>=0;q--)
        for(int d=(val-q*25)/10;d>=0;d--)
            for(int n=(val-q*25-d*10)/5;n>=0;n--)
                printf("%d quarter(s), %d dime(s), %d nickel(s), %d pennie(s)\n", q, d, n, val-q*25-d*10-n*5);

    return 0;

}
