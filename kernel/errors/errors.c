#include <stdarg.h>

#include "errors.h"

// Return the error message
const char *get_last_error()
{
    return error_message;
}

// Set the error message
void set_error(const char *message, ...)
{
    va_list args;
    va_start(args, message);

    // Make sure the error message doesn't exceed the length of the buffer
    if (strlen(error_message) >= ERROR_MESSAGE_LEN) {
        printf("Error: error message has to be less than %d characters\n", ERROR_MESSAGE_LEN);
        return;
    }

    unsigned int j;
	unsigned int num_chars_written = 0;

	for (j=0; j<strlen(message); j++) {
		/* check for format specifier */

		if (message[j] == '%') {
			/* print a number to the screen */

			if (message[j+1] == 'd') {
				int i = va_arg(args, int);
				char* str = "";

				if (i == 0 || i == '0') {
					strcat(error_message, "0");
				} else {
					// Convert the integer to a string, increase num_chars_written and add the integer to the error message
					itoa(i, str, 10);

					strcat(error_message, str);
				}
			}

			/* prints a character to the screen */

			else if (message[j+1] == 'c') {
				int c = va_arg(args, int);

				strcat(error_message, (const char*) c);
			}

			/* prints a string to the screen */

			else if (message[j+1] == 's') {
				int s;
				s = va_arg(args, int);

				strcat(error_message, (const char*) s);
			}

			/* check if number is to be converted to hex */

			else if (message[j+1] == 'x' || message[j+1] == 'X') {

				int j = 0;

				int i = va_arg(args, int);
				char* str = "";
				itoa(i, str, 10);
				// Set the maximum number of characters to the length of the number as a string, leaving a space for the null byte
				char hex[strlen(str)];

				// Print hex characters in lowercase if lowercase x was used, otherwise print hex in uppercase
				if (message[j+1] == 'x') {
					char hex_chars_lower[] = "0123456789abcdef";

					do {
						hex[j++] = hex_chars_lower[i%16];
					} while ((i/=16) > 0);
				} else {
					char hex_chars_upper[] = "0123456789abcdef";

					do {
						hex[j++] = hex_chars_upper[i%16];
					} while ((i/=16) > 0);
				}

				strrev(hex);

				hex[j+1] = '\0';

				strcat(error_message, hex);
			}

			/* check if pointer is to be printed */

			else if (message[j+1] == 'p') {
				//void* ptr = va_arg(args, void*);

				// TODO: implementation needed
			}

			/* prints a percent (%) */

			else if (message[j+1] == '%') {
				num_chars_written++;
				strcat(error_message, "%");
			}

			/* prints a new line (cursor goes to the beginning of the next line) */

			else if (message[j] == '\n') {
				strcat(error_message, "\n");
			}

			/* prints a tab character */

			else if (message[j] == '\t') {
				strcat(error_message, "\t");
			}
		}

		/* else, print the character */

		else if (message[j-1] != '%') {
			strcat(error_message, (const char*) ((char*) message[j]));
		}
	}

    // Set the error message to the `message` parameter
    //strcpy(error_message, message);
    error_message[strlen(error_message)] = '\0';

    va_end(args);
}