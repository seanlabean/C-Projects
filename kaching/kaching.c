#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    float s = 0, t = 0, c = 0;
    char input[100];  

    printf("Yearly Salary | $");

    if (fgets(input, sizeof(input), stdin) == NULL) 
    {
        printf("Error reading input.\n");
        return 1;
    }
    if (!(sscanf(input, "%f", &s) == 1)) 
    {
        printf("Invalid input. Please enter a valid float.\n");
    } 
    t = s / 12 / 20 / 8 / 60 / 60;
    putchar('\n');

    do {
        printf("\r");
        printf("\033[ARaked In: $%.2f\n\r", c); 
        sleep(1);
        c += t;
    } while (c < t * 60 * 60 * 8);

    printf("One day's pay.\n");

    return 0;
}
