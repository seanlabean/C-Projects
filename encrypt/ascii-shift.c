#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFF_SIZE 255

bool isNumber(char number[]) {
    int i = 0;
    if (number[0] == '-') {i=1;}
    for (; number[i] != 0; i++) {
        if (!isdigit(number[i])) {return false;}
    }
    return true;
}

void main(int argc, char* argv[]) {
    FILE *fin, *fout;
    int i,n;

    char buffer[BUFF_SIZE];

    if (!isNumber(argv[3])) {
        printf("%s shift given is not a number.\n",argv[3]);
        exit(1);
    }

    fin = fopen(argv[1],"r");
    if(fin == NULL) {
        printf("%s file does not exist.\n",argv[1]);
        exit(1);
    }

    fout = fopen(argv[2],"w");
    if(fout == NULL) {
        printf("%s file could not be created.\n",argv[2]);
        exit(1);
    }

    while(fgets(buffer, sizeof(buffer),fin)!=0) {
        n = strlen(buffer);
        for(i=0; i<n; i++) {
            buffer[i] = buffer[i] + atoi(argv[3]);
        }
        fputs(buffer,fout);
    }
    fclose(fin);
    fclose(fout);
    printf("File '%s' encrypted by ASCII shift of %s.\nSaved to '%s'.\n",argv[1],argv[3],argv[2]);
}