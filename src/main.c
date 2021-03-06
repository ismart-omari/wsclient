#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wsclient.h"

#ifdef CLIENT_CERT
static const char * const CA_CERT_FILE = "etc/cacert.pem";
static const char * const CLIENT_CERT_FILE = "etc/c1-testclient01.crt.pem";
static const char * const CLIENT_KEY_FILE = "etc/c1-testclient01.key.pem";
static const char * const CLIENT_PASSWD = "";
#else
static const char * const CA_CERT_FILE = "etc/cacert_s.pem";
#endif

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

	ws_set_ca_cert(CA_CERT_FILE);
#ifdef CLIENT_CERT
	ws_set_client_cert(CLIENT_CERT_FILE, CLIENT_KEY_FILE, CLIENT_PASSWD);
#endif
	ws = ws_connect(argv[optind], proto);
	if (ws) {
		printf("Connected.\n");
		ws_close(ws);
		ws_unset_client_cert();
		ws_unset_ca_cert();
	}
	else {
		printf("Failed to connect.\n");
	}
	return EXIT_SUCCESS;
}

