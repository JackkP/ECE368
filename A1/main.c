/* 
 * Jack Porter, Purdue ECE368
 * Assignment 1
 *
 */

#include <stdio.h>

int main(int argc, char ** argv)
{
    // read in the change to make
    int val;
    scanf("%d", &val);
    // count down from the maximum number of possible each coin (except pennies) and print
    for (int q=val/25;q>=0;q--)
        for(int d=(val-q*25)/10;d>=0;d--)
            for(int n=(val-q*25-d*10)/5;n>=0;n--)
                printf("%d quarter(s), %d dime(s), %d nickel(s), %d pennie(s)\n", q, d, n, val-q*25-d*10-n*5);

    return 0;

}
