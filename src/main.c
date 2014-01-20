#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wsclient.h"

static void
print_usage(void)
{
	fprintf(stderr, "usage: wsclient [-p proto] [-s proxy:port] <server_url>\n");
}

static void
set_proxy(const char *url)
{
	const char *proxy = url;
	char *p;
	uint16_t port = 0;

	p = strchr(url, ':');
	if (p) {
		*p = '\0';
		port = atoi(p + 1);
	}
	ws_set_proxy(proxy, port);
}

int
main(int argc, char *argv[])
{
	struct ws *ws;
	int c;
	const char *proto = NULL;

	while ((c = getopt(argc, argv, "p:s:")) != -1) {
		switch (c) {
		case 'p':
			proto = optarg;
			break;
		case 's':
			set_proxy(optarg);
			break;
		default:
			print_usage();
			return EXIT_FAILURE;
		}
	}

	if (!argv[optind]) {
		print_usage();
		return EXIT_FAILURE;
	}

	ws_set_passwd("");
	ws = ws_connect(argv[optind], proto);
	if (ws) {
		printf("Connected.\n");
		ws_close(ws);
	}
	else {
		printf("Failed to connect.\n");
	}
	return EXIT_SUCCESS;
}

