#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cubehash.h"

static void usage(char *error)
{
	bool iserror = error || errno;

	if (iserror && strlen(error))
		warn(error);

	fprintf(
		iserror ? stderr : stdout,
		"%s"
		"Usage: cubehash [OPTIONS] [PARAMETERS]                                        \n"
		"                                                                              \n"
		"OPTIONS                                                                       \n"
		"  -2    Use the second revision of proposed CubeHash parameters, implementing \n"
		"        CubeHash160+16/32+160-h for hash length h.                            \n"
		"  -3    Use the third revision of proposed CubeHash parameters, implementing  \n"
		"        CubeHash16+16/32+32-h for hash length h. This is the default.         \n"
		"  -l HASHLEN                                                                  \n"
		"        Set the hash length in bits (default: 256). The hash length must be   \n"
		"        positive, evenly divisible by 8, and not greater than 512.            \n"
		"  -h    Show this help text and exit.                                         \n",
		iserror ? "\n" : ""
	);

	exit(iserror);
}

int main(int argc, char *argv[])
{
	bool done = false;
	int hashlen = 256, revision = 3;
	char *endptr;

	while (!done)
		switch (getopt(argc, argv, "23l:h")) {
			case '2':
				revision = 2;
				break;
			case '3':
				revision = 3;
				break;
			case 'l':
				hashlen = (int) strtol(optarg, &endptr, 0);

				if (&optarg[strlen(optarg)] != endptr)
				{
					errno = EINVAL;
					usage("-l HASHLEN");
				}

				break;
			case 'h':
				usage(NULL);
				break;
			case -1:
				done = true;
				break;
			default:
				usage("");
		}

	uint8_t *hash = cubehash(stdin, revision, hashlen);

	if (errno == EDOM)
		usage("-l HASHLEN");

	for (int i = 0; i < hashlen / 8; i++)
		printf("%02x", hash[i]);

	puts("");
}
