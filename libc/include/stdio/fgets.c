#include "stdio.h"

// Read characters from stream into str, num times, or until a newline character
// or the file EOF is reached
char* fgets(char* str, int num, FILE* stream) {
    if (stream == NULL || stream->eof == EOF)
        return NULL;

    // Add each read character to the string str.
    // Exit the loop if a newline character ot EOF is encountered
    for (int i = 0; i < num; i++) {
        if (stream->eof != EOF) {
            int c = fgetc(stream);
            str[i] = c;

            if (c == '\n') {
                break;
            }
        } else {
            break;
        }
    }

    return str;
}