#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char ** argv) {

    FILE *file = fopen("test3.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Seed the random number generator
    srand(time(NULL));

    if (argc < 4) return 1; 

    int xmax = atoi(argv[1]);
    int ymax = atoi(argv[2]);
    int npoints = atoi(argv[3]);

    // Generate and write 10,000 random points
    int i;
    for (i = 0; i < npoints; i++) {
        int x = rand() % (xmax*2+1) - xmax; // Random value between -xmax and xmax
        int y = rand() % (ymax*2+1) - ymax; // Random value between -10000 and 10000
        fprintf(file, "%d %d\n", x, y);
    }

    fclose(file); // Close the file
    printf("Generated %d random points and saved to test3.txt\n", npoints);
    
    return 0;
}