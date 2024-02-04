#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to copy content from source file to destination file using a buffer
void copyFile(FILE *source, FILE *destination, size_t bufferSize) {
    char *buffer = (char *)malloc(bufferSize);
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, bufferSize, source)) > 0) {
        fwrite(buffer, 1, bytesRead, destination);
    }

    free(buffer);
}

int main(int argc, char *argv[]) {
    // Check for minimum number of arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-bnumber] file1 file2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize parameters
    size_t bufferSize;
    int argIndex = 1;

    if (strcmp(argv[1], "-b") == 0) {
        // If -b parameter is provided, set the buffer size
        bufferSize = atoi(argv[2]);
        argIndex = 3;
    } else {
        // If -b parameter is not provided, use the system default buffer size
        bufferSize = 0;  // 0 indicates using the system's default buffer size
    }

    // Check for sufficient number of arguments after buffer check
    if (argc - argIndex != 2) {
        fprintf(stderr, "Incorrect syntax. Usage: %s [-bnumber] file1 file2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open files
    FILE *sourceFile = fopen(argv[argIndex], "rb");
    FILE *destinationFile = fopen(argv[argIndex + 1], "wb");

    // Check for successful file openings
    if (sourceFile == NULL || destinationFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    // If bufferSize is 0, use the system default buffer size
    if (bufferSize == 0) {
        fseek(sourceFile, 0L, SEEK_END);
        bufferSize = (size_t)ftell(sourceFile);
        rewind(sourceFile);
    }

    // Measure the copying speed
    clock_t start = clock();
    copyFile(sourceFile, destinationFile, bufferSize);
    clock_t end = clock();
    double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;

    // Print results
    printf("Copy speed: %.2f MB/s\n", (double)ftell(sourceFile) / (elapsedTime * 1024 * 1024));

    // Close files
    fclose(sourceFile);
    fclose(destinationFile);

    // Log results to a report.out file
    FILE *reportFile = fopen("report.out", "a");
    fprintf(reportFile, "Buffer size: %zu, Copy speed: %.2f MB/s\n", bufferSize, (double)ftell(sourceFile) / (elapsedTime * 1024 * 1024));
    fclose(reportFile);

    return 0;
}

