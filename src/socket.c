#include "commons.h"

/*
 * Initializes the ICMP socket, and configures the destination address
*/
int	initialize_icmp_socket(t_traceroute_exec *exec)
{
	struct addrinfo	hints, *result;

	exec->socket_fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (exec->socket_fd == -1)
	{
		perror("socket"); //TODO: replace to remove forbidden function
		return (1);
	}
	
	ft_memset(&exec->dest_addr, 0, sizeof(exec->dest_addr));
	exec->dest_addr.sin_family = PF_INET;
	if (inet_pton(PF_INET, exec->context->dest_host, &exec->dest_addr.sin_addr) <= 0)
	{
		ft_memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_RAW;
		
		if (getaddrinfo(exec->context->dest_host, NULL, &hints, &result) != 0)
		{
			fprintf(stderr, "ft_traceroute: unknown host %s\n", exec->context->dest_host);
			close(exec->socket_fd);
			return (1);
		}
		ft_memcpy(&exec->dest_addr, result->ai_addr, sizeof(struct sockaddr_in));
		freeaddrinfo(result);
	}
	return (0);
}

