#include <stdio.h>

int main() {
    FILE *fp;
    int num = 10;  // the integer you want to write

    // open file in binary write mode
    fp = fopen("player_ids.dat", "wb");  
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // write the integer to the file
    fwrite(&num, sizeof(int), 1, fp);

    fclose(fp);
    printf("Integer written to data.bin successfully!\n");

    return 0;
}
