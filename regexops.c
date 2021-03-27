/*
* BSD 2-Clause License
*
* Copyright (c) 2021, uidops
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#include <err.h> /* 
				* for use: void errx(int eval, const char *fmt, ...)
				*/
#include <getopt.h>
#include <regex.h> /*
					* for use: int regcomp(regex_t *_restrict_ preg, const char *_restrict_ pattern,
					* 					int cflags)
					*
					* and: int regexec(const regex_t *preg, const char *string,
            		* 					size_t nmatch, regmatch_t *pmatch, int eflags)
					*/
 
#include <signal.h> /*
					* for signal handling.  
					*/

#include <stdbool.h>  /*
					* for use Boolean data type.
					*/

#include <stdio.h>  /*
					* for use:
					*  int fprintf(FILE *restrict stream,
					*				const char *restrict format, ...)
					*
					* FILE *fopen(const char *restrict pathname, const char *restrict mode)
					* FILE *fdopen(int fd, const char *mode)
					*
					* int fclose(FILE *stream)
					* 
					* ssize_t getline(char **restrict lineptr, size_t *restrict n,
					* 					FILE *restrict stream);
					*
					* int feof(FILE *stream)
					*/

#include <stdlib.h> /*
					* for use: 
					* void free(void *ptr) 
					* void exit(int status)
					*/

#include <string.h> /*
					* for use:
					* int strcmp(const char *s1, const char *s2)
					*/

#include <unistd.h> /*
					* for use:
					* int isatty(int fd)
					*/


#define BUG_ADDRESS "<uidopsu@gmail.com>" /* Email for report bug.  */
#define VERSION_ "1.1" /* Version of program.  */
#define HOME_PAGE "https://github.com/siruidops/regexops" /* Home page.  */ 


void		 print_arg_error(void); /* print_arg_error declaration  */
void		 signal_handler(int); /* signal_handler declaration  */

extern char		*__progname; /* __progname is defined by the libc for get program name. */

struct option long_opt[] = {
	{"number", 0, NULL, 'n'},
	{"help", 0, NULL, 'h'},
	{"usage", 0, NULL, 777},
	{"version", 0, NULL, 'V'},
	{NULL, 0, NULL, 0}
};

typedef struct Options {
    bool		 show_number;
	char		*filename;
	char		*regex;
} Options;
struct Options setting = {false, NULL, NULL};

void
signal_handler(int signum)
{ /* Signal handler.  */
	exit(1);
}

void
print_arg_error()
{
	/* a function for write a banner for invalid argument in to standard error.  */
	fprintf(stderr, "Usage: regexops [OPTION...] [REGEX_PATTERN] [FILE]\nTry `%s --help' or `%s --usage' for more information.\n", __progname,  __progname);
	exit(1); /* exit with status number 1.  */
}

int
main(int argc, char *argv[])
{
	regex_t		 regexer;
	int		 value, opt; /* declaring Integer for recive status of  regcomp and regexec for handle errors. and opt. */
	long int		 line_number = 0; /* declaring Integer for Count the number of lines.  */

	char		*line = NULL; /* declaring character pointer for recive line.  */ 
	size_t		 len = 0;
	FILE		*fp; /* declaring pointer of FILE type.  */

	while((opt = getopt_long(argc, argv, ":nhV", long_opt, NULL)) != -1) {
		switch(opt) {
			case -1: /* no more arguments  */
			case 0: /* long options toggles  */
				break;
			case 'n':
				setting.show_number = true;
				break;

			case 'h':
				fprintf(stdout, "Usage: %s [OPTION...] [REGEX_PATTERN] [FILE]\n", __progname);
				fprintf(stdout, "a small tool for printing lines that match Regex patterns.\n\n");
				fprintf(stdout, " Options:\n");
				fprintf(stdout, "   -n, --number             Show line number\n");
				fprintf(stdout, "   -h, --help               Give help message\n");
				fprintf(stdout, "   --usage                  Give a short usage message\n");
				fprintf(stdout, "   -V, --version            Print program version\n\n");
				fprintf(stdout, "Report bugs to: %s\n", BUG_ADDRESS);
				fprintf(stdout, "Regexops home page: %s\n", HOME_PAGE);
				exit(0);

			case 777:
				fprintf(stdout, "Usage: regexops [-n?V] [--number] [--help] [--usage] [--version] [REGEX_PATTERN] [FILE]\n");
				exit(0);
			
			case 'V':
				fprintf(stdout, "Regexops version %s\n", VERSION_);
				exit(0);

			default:
				fprintf(stderr, "%s: invalid option -- %c\n", __progname, opt);
				fprintf(stderr, "Try `%s --help' or `%s --usage' for more information.\n", __progname, __progname);
				exit(1);
		};
	};

	for(; optind < argc; optind++) {
		if (setting.regex == NULL)
			setting.regex = argv[optind];
		else
			setting.filename = argv[optind];
    }


	signal(SIGINT, signal_handler); /* Handle SIGINT Signal.  */
	signal(SIGKILL, signal_handler); /* Handle SIGKILL Signal.  */

	if (!feof(stdin) && !isatty(0)) { /* Check stdin is not a terminal and not EOF.  */
		if (argc < 2 || setting.regex == NULL) {
			setting.regex = ".";
		}
		argc += 1;

		fp = fdopen(0, "r"); /* Convert file descriptor to STREAM.  */

	} else {
		if (setting.filename == NULL || argc < 3)
			print_arg_error();

		fp = fopen(setting.filename, "r"); /* Open file as STREAM.  */
		if (fp == NULL) /* Check whether it was opened successfully or not.  */
			errx(1, "Can't open file."); /*‌ Print error and exit with status number 1.  */
	}
	

	value = regcomp(&regexer, setting.regex, 0); /* Compiles the regular expression specified by pattern into an executable string of op-codes.  */
	if (value != 0) /* Check whether it was successfully or not. */
		errx(1, "Creating regcomp failed.\n"); /*‌ Print error and exit with status number 1.  */

	while (getline(&line, &len, fp) != -1) { /* Read file line-by-line.  */
		line_number += 1; /* Append 1 to line_number.  */
		value = regexec(&regexer, line, 0, NULL, 0); /* Compares the NULL-terminated string specified by string against the compiled regular expression.  */
		if (value == 0) {/* Check to see if the pattern is in the text.  */
			if (setting.show_number)
				fprintf(stdout, "%ld:%s", line_number, line); /* Write line to standard output with line number.  */
			else
				fprintf(stdout, "%s", line); /* Write line to standard output.  */
		}
	}

	fclose(fp); /* Close opened stream file.  */

	if (line)
		free(line); /* free dynamic memory.  */

	return 0;
}

/*
* Thanks for reading.
*/
