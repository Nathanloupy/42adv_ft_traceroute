#pragma once

/* LIBS */
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
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
#define MAXIPLEN 60
#define MAXICMPLEN 76
#define TRACEROUTE_MAX_DATALEN (65535 - MAXIPLEN - MAXICMPLEN)
#define ICMP_DATA_PATTERN_MODULO 256

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
	const t_traceroute_context	*context;
	pid_t						pid;
	int							socket_fd;
	struct sockaddr_in			dest_addr;
	int							dest_reached;
	int							current_ttl;
	int							hop_index;
	struct icmphdr				icmp_hdr;
	int							seq_num;
	char						packet[TRACEROUTE_MAX_DATALEN];
	size_t						packet_size;
	struct timeval				send_time;
	struct timeval				recv_time;
}	t_traceroute_exec;

/* RETURN CODES */
enum e_ft_traceroute_ret_code
{
	FT_TRACEROUTE_ERROR = -1,
	FT_TRACEROUTE_OK = 0,

	FT_TRACEROUTE_TIMEOUT = 1,
	FT_TRACEROUTE_NO_REPLY = 2,
};

/* PARSER */
int		parse_arguments(int argc, char *argv[], t_traceroute_context *context);

/* SOCKET */
int		initialize_icmp_socket(t_traceroute_exec *exec);

/* HEADER */
int		print_header(t_traceroute_exec *exec);

/* CREATE PACKET */
void	create_packet(struct icmphdr *icmp_hdr, char *packet, int seq_num, size_t packet_size, t_traceroute_exec *exec);

/* EXECUTE TRACEROUTE */
int		execute_traceroute(t_traceroute_exec *exec);

/* SEND PACKET */
int		send_packet(t_traceroute_exec *exec);

/* WAIT PACKET */
int		wait_packet(t_traceroute_exec *exec);