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

#include <sys/select.h> /*
						* for use: int select(int nfds, fd_set *restrict readfds,
						*					fd_set *restrict writefds, fd_set *restrict errorfds,
						*					struct timeval *restrict timeout)
						*/
#include <err.h> /* 
				*for use: void errx(int eval, const char *fmt, ...)
				*/

#include <regex.h> /*
					* for use: int regcomp(regex_t *_restrict_ preg, const char *_restrict_ pattern,
					* 					int cflags)
					*
					* and: int regexec(const regex_t *preg, const char *string,
            		* 					size_t nmatch, regmatch_t *pmatch, int eflags)
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

void		 print_arg_error(char*); /* print_arg_error(char*) declaration  */
void		 print_arg_help(char*); /* print_arg_help(char*) declaration  */

void
print_arg_error(char *filename)
{
	/* a function for write a banner for invalid argument in to standard error.  */
	fprintf(stderr, "Usage: %s REGEX_PATTERN [FILE]\nTry '%s --help' for more information.\n", filename, filename);
	exit(1); /* exit with status number 1.  */
}

void
print_arg_help(char *filename)
{
	/* a function for write a banner for --help into standard output.  */

	fprintf(stdout, "REGEXOPS - A small tool for printing lines that match Regex patterns.\n\n");
	fprintf(stdout, "Usage: %s REGEX_PATTERN [FILE]\n", filename);
	fprintf(stdout, "Example: %s ^a /tmp/a.txt\n\n", filename);
	fprintf(stdout, "You can also pass data from the standard input to the tool:\n");
	fprintf(stdout, "cat /tmp/a.txt | %s ^a\n\n", filename);
	fprintf(stdout, "License: BSD 2-Clause\n");
	fprintf(stdout, "Soruce code: https://github.com/siruidops/regexops\n");
	exit(0); /* exit with status number 0(sucess).  */
}

int
main(int argc, char *argv[])
{
	regex_t		 regexer;
	int		 value; /* declaring Integer for recive status of  regcomp and regexec for handle errors.  */

	char		*line = NULL; /* declaring character pointer for recive line.  */ 
	size_t		 len = 0;
	FILE		*fp; /* declaring pointer of FILE type.  */

	fd_set		 readfs;


	if (argc > 1 && strcmp(argv[1], "--help") == 0) /* Check if --help inside arguments.  */
		print_arg_help(argv[0]);

	FD_ZERO(&readfs); /* removes all file descriptors from set.  */
	FD_SET(0, &readfs); /* add stdin file descriptor to set.  */

	/*
	* timeout structer for select.
	*/
	struct timeval timeout;
	timeout.tv_sec = 0; /* seconds */
	timeout.tv_usec = 1; /* microseconds */

	select(1+1, &readfs, NULL, NULL, &timeout); /* Wait for the file descriptors to be ready.  */

	if (FD_ISSET(0, &readfs)) { /* Check stdin is ready for read.  */
		fp = fdopen(0, "r"); /* Convert file descriptor to STREAM.  */
		
		if (argc != 2)
			print_arg_error(argv[0]);
		else
			argc = 3;

	} else {
		
		if (argc != 3)
			print_arg_error(argv[0]);

		fp = fopen(argv[2], "r"); /* Open file as STREAM.  */
		if (fp == NULL) /* Check whether it was opened successfully or not.  */
			errx(1, "Can't open file."); /*‌ Print error and exit with status number 1.  */
	}

	value = regcomp(&regexer, argv[1], 0); /* Compiles the regular expression specified by pattern into an executable string of op-codes.  */
	if (value != 0) /* Check whether it was successfully or not. */
		errx(1, "Creating regcomp failed.\n"); /*‌ Print error and exit with status number 1.  */

	while (getline(&line, &len, fp) != -1) { /* Read file line-by-line.  */
		value = regexec(&regexer, line, 0, NULL, 0); /* Compares the NULL-terminated string specified by string against the compiled regular expression.  */
		if (value == 0) /* Check to see if the pattern is in the text.  */
			fprintf(stdout, "%s", line); /* Write line to standard output.  */
	}

	fclose(fp); /* Close opened stream file.  */

	if (line)
		free(line); /* free dynamic memory.  */

	return 0;
}

/*
* Thank you for reading.
*/
