#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char ** argv) {
    FILE *file = fopen("test.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Generate and write 10,000 random points
    for (int i = 0; i < 10000; i++) {
        int x = rand() % 20001 - 10000; // Random value between -10000 and 10000
        int y = rand() % 20001 - 10000; // Random value between -10000 and 10000
        fprintf(file, "%d %d\n", x, y);
    }

    fclose(file); // Close the file
    printf("Generated 10,000 random points and saved to test.txt\n");
    
    return 0;
}