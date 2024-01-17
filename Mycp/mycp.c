#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void copyFile(FILE *source, FILE *destination, size_t bufferSize) {
    char *buffer = (char *)malloc(bufferSize);
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, bufferSize, source)) > 0) {
        fwrite(buffer, 1, bytesRead, destination);
    }

    free(buffer);
}

int main(int argc, char *argv[]) {
    // Έλεγχος για ελάχιστο αριθμό ορισμάτων
    if (argc < 3) {
        fprintf(stderr, "Χρήση: %s [-bnumber] file1 file2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Αρχικοποίηση παραμέτρων
    size_t bufferSize;

    int argIndex = 1;

    if (strcmp(argv[1], "-b") == 0) {
        // Εάν δοθεί η παράμετρος -b, καθορίζουμε το μέγεθος του buffer
        bufferSize = atoi(argv[2]);
        argIndex = 3;
    } else {
        // Αν δεν δοθεί η παράμετρος -b, χρησιμοποιούμε το μέγεθος που παρέχεται από το σύστημα
        bufferSize = 0;  // 0 ως ένδειξη να χρησιμοποιηθεί το default μέγεθος του συστήματος
    }

    // Έλεγχος για επαρκές αριθμό ορισμάτων μετά τον έλεγχο του buffer
    if (argc - argIndex != 2) {
        fprintf(stderr, "Λανθασμένη σύνταξη. Χρήση: %s [-bnumber] file1 file2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Άνοιγμα αρχείων
    FILE *sourceFile = fopen(argv[argIndex], "rb");
    FILE *destinationFile = fopen(argv[argIndex + 1], "wb");

    // Έλεγχος για επιτυχία άνοιγματος αρχείων
    if (sourceFile == NULL || destinationFile == NULL) {
        perror("Σφάλμα κατά το άνοιγμα των αρχείων");
        exit(EXIT_FAILURE);
    }

    // Εάν το bufferSize είναι 0, χρησιμοποιούμε το default μέγεθος του συστήματος
    if (bufferSize == 0) {
        fseek(sourceFile, 0L, SEEK_END);
        bufferSize = (size_t)ftell(sourceFile);
        rewind(sourceFile);
    }

    // Μέτρηση της ταχύτητας αντιγραφής
    clock_t start = clock();
    copyFile(sourceFile, destinationFile, bufferSize);
    clock_t end = clock();
    double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;

    // Εκτύπωση αποτελεσμάτων
    printf("Ταχύτητα αντιγραφής: %.2f MB/s\n", (double)ftell(sourceFile) / (elapsedTime * 1024 * 1024));

    // Κλείσιμο αρχείων
    fclose(sourceFile);
    fclose(destinationFile);

    // Καταγραφή των αποτελεσμάτων σε ένα αρχείο report.out
    FILE *reportFile = fopen("report.out", "a");
    fprintf(reportFile, "Μέγεθος buffer: %zu, Ταχύτητα αντιγραφής: %.2f MB/s\n", bufferSize, (double)ftell(sourceFile) / (elapsedTime * 1024 * 1024));
    fclose(reportFile);

    return 0;
}
