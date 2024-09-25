#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "sample.txt"

void* count_lines(void* arg) {
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int line_count = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }
    fclose(fp);

    printf("Total lines: %d\n", line_count);
    return NULL;
}

void* replace_chars(void* arg) {
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Read the content of the file into a buffer
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0'; // Null-terminate the string
    fclose(fp);

    // Open the file for writing the modified content
    fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        perror("Error opening file for writing");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // Perform character replacements and write back to the file
    for (size_t i = 0; i < file_size; i++) {
        switch (buffer[i]) {
            case 's':
                fputs("/s", fp);
                break;
            case 'S':
                fputs("/S", fp);
                break;
            case 'P':
            case 'p':
                fputc('F', fp); 
                break;
            default:
                fputc(buffer[i], fp);
        }
    }

    fclose(fp);
    free(buffer);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, count_lines, NULL) != 0) {
        perror("Error creating thread 1");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread2, NULL, replace_chars, NULL) != 0) {
        perror("Error creating thread 2");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("File modification and line count complete.\n");
    return 0;
}
