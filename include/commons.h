#pragma once

/* LIBS */
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "lpyp.h"

/* UTILS */
#include "utils.h"

/* DEFINES - GENERAL */
#define DEFAULT_INITIAL_TTL 1
#define DEFAULT_MAX_TTL 64
#define DEFAULT_TRIES_PER_HOP 3
#define DEFAULT_WAIT_RESPONSE 3

/* DEFINES - FLAGS */
#define FLAG_RESOLVE_HOSTNAMES	(1 << 0) /* -r, --resolve-hostnames flag */

/* STRUCTS */
typedef struct s_traceroute_context
{
	int				initial_ttl;
	int				max_ttl;
	int				tries_per_hop;
	int				wait_response;
	unsigned int	flags;
	char			*dest_host;
}	t_traceroute_context;

typedef struct s_traceroute_exec
{
	pid_t					pid;
	int						socket_fd;
	struct sockaddr_in		dest_addr;
}	t_traceroute_exec;

/* PARSER */
int		parse_arguments(int argc, char *argv[], t_traceroute_context *context);

/* SOCKET */
int		initialize_icmp_socket(t_traceroute_context *context, t_traceroute_exec *exec);

/* HEADER */
int		print_header(t_traceroute_context *context);
