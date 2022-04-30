#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define streq(a, b) (strcmp(a, b) == 0)

void usage(const char* prog) {
	printf(
"Usage: %s <id> [command]\n"
"\n"
"Root-only utility to run a command with a given numeric user id (short for\n"
"\"numeric su\")\n"
"This doesn't check whether the id is registered, so you can use an arbitrary integer.\n"
	, prog);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}
	else if (streq(argv[1], "-h") || streq(argv[1], "--help")) {
		usage(argv[0]);
		return 0;
	}

	char* e;
	int uid = strtol(argv[1], &e, 10);
	if (*e != '\0') {
		fprintf(stderr, "Invalid user-id (must be a number)\n");
		return 1;
	}

	int err = setuid(uid);
	if (err) {
		perror("Couldn't set user-id (must be root or have CAP_SETUID)");
		return 1;
	}

	char* file = argc >= 3? argv[2] : "/bin/sh";

	int extra_args = argc - 3 >= 0? argc - 3 : 0;
	char* args[extra_args + 2];
	args[0] = file;
	for (int i = 0; i < extra_args; i++) {
		args[i + 1] = argv[i + 3];
	}
	args[extra_args + 1] = NULL;

	char* envp[] = { "PATH=/bin/:/sbin/:/usr/bin", NULL };

	execvpe(file, args, envp);

	// execvpe shouldn't return; if we get here, an error happened.
	perror("execvpe failed");
}
