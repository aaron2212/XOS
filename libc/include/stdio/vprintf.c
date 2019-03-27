#include <stdarg.h>

#include "stdio.h"
#include "../size_t.h"

extern size_t strlen(const char *str);
extern char *strrev(char *str);

int vprintf(const char *fmt, va_list args)
{
	unsigned int j;
	unsigned int num_chars_written = 0;

	for (j = 0; j < strlen(fmt); j++)
	{
		/* check for format specifier */

		if (fmt[j] == '%')
		{
			j++;
			/* print a number to the screen */

			if (fmt[j] == 'd')
			{
				int i = va_arg(args, int);
				char *str = "";

				// if (strncmp("0x", (char *) i, 2) == 0) { //convert to decimal }

				if (i == 0 || i == '0')
				{
					puts("0");
				}
				else
				{
					// Convert the integer to a string, increase num_chars_written and print the integer
					itoa(i, str, 10);
					num_chars_written += strlen(str);

					puts(str);
				}
			}

			/* prints a character to the screen */

			else if (fmt[j] == 'c')
			{
				int c = va_arg(args, int);

				num_chars_written++;
				putchar(c);
			}

			/* prints a string to the screen */

			else if (fmt[j] == 's')
			{
				char *s = va_arg(args, char *);
				num_chars_written += strlen(s);

				puts(s);
			}

			/* check if number is to be converted to hex */

			else if (fmt[j] == 'x' || fmt[j] == 'X')
			{

				int j = 0;

				int i = va_arg(args, int);
				char *str = "";
				itoa(i, str, 10);
				// Set the maximum number of characters to the length of the number as a string, leaving a space for the null byte
				char hex[strlen(str)];

				// Print hex characters in lowercase if lowercase x was used, otherwise print hex in uppercase
				if (fmt[j] == 'x')
				{
					char hex_chars_lower[] = "0123456789abcdef";

					do
					{
						hex[j++] = hex_chars_lower[i % 16];
					} while ((i /= 16) > 0);
				}
				else
				{
					char hex_chars_upper[] = "0123456789abcdef";

					do
					{
						hex[j++] = hex_chars_upper[i % 16];
					} while ((i /= 16) > 0);
				}

				hex[j] = '\0';
				strrev(hex);

				hex[j + 1] = '\0';

				num_chars_written += strlen(hex);

				puts(hex);
			}

			/* check if pointer is to be printed */

			else if (fmt[j] == 'p')
			{
				void *ptr = va_arg(args, void *);

				kprintf("%d", ptr);
			}

			/* prints a percent (%) */

			else if (fmt[j] == '%')
			{
				num_chars_written++;
				putchar('%');
			}

			/* prints a new line (cursor goes to the beginning of the next line) */

			else if (fmt[j - 1] == '\n')
			{
				// Calls to kprintf() with '\n' result in "\r\n" being printed
				num_chars_written += 1;

				putchar('\n');
			}

			/* prints a tab character */

			else if (fmt[j - 1] == '\t')
			{
				num_chars_written++;

				putchar('\t');
			}
		}

		/* else, print the character */

		else if (fmt[j - 1] != '%')
		{
			num_chars_written++;

			putchar(fmt[j]);
		}
	}

	return num_chars_written;
}
