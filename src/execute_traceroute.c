#include "commons.h"

/*
 * Helper function to print the address depending on whether or not the resolving of hostnames is enabled
 */
static int	print_address(t_traceroute_exec *exec, struct sockaddr_in *addr)
{
	char	hostname[NI_MAXHOST];
	int		result;

	if (!inet_ntoa(addr->sin_addr))
		return (1);
	
	if (exec->context->flags & FLAG_RESOLVE_HOSTNAMES)
	{
		result = getnameinfo((struct sockaddr*)addr, sizeof(*addr), 
							hostname, sizeof(hostname), NULL, 0, 0);
		if (result == 0)
			printf("%s (%s)  ", inet_ntoa(addr->sin_addr), hostname);
		else
			printf("%s (%s)  ", inet_ntoa(addr->sin_addr), inet_ntoa(addr->sin_addr));
	}
	else
	{
		printf("%s  ", inet_ntoa(addr->sin_addr));
	}
	return (0);
}

/*
 * Helper function to handle a single try
*/
static int	single_try(int *is_first, int *is_first_successful, t_traceroute_exec *exec)
{
	int	result;

	create_packet(&exec->icmp_hdr, exec->packet, exec->seq_num, exec->packet_size, exec);
	if (send_packet(exec) < 0)
	{
		perror("sendto"); //TODO: replace to remove forbidden function
		return (1);
	}
	result = wait_packet(exec);
	switch (result)
	{
		case FT_TRACEROUTE_OK:
			if (*is_first)
			{
				printf("%3d   ", exec->hop_index);
				*is_first = 0;
			}
			if (*is_first_successful || exec->hop_addr.sin_addr.s_addr != exec->last_hop_addr.sin_addr.s_addr)
			{
				if (print_address(exec, &exec->hop_addr))
					return (1);
				*is_first_successful = 0;
				exec->last_hop_addr = exec->hop_addr;
			}
			printf("%.3fms  ", exec->response_time_ms);
			fflush(stdout);
			return (0);
		case FT_TRACEROUTE_TIMEOUT:
			if (*is_first)
			{
				printf("%3d  ", exec->hop_index);
				*is_first = 0;
			}
			printf("*  ");
			fflush(stdout);
			break;
		default:
			return (1);
	}
	return (0);
}

/*
 * Helper function to handle a single hop
*/
static int	single_hop(t_traceroute_exec *exec)
{
	int	is_first = 1;
	int	is_first_successful = 1;
	int	temp_ttl = exec->current_ttl;

	ft_memset(&exec->last_hop_addr, 0, sizeof(exec->last_hop_addr));
	if (setsockopt(exec->socket_fd, IPPROTO_IP, IP_TTL, &temp_ttl, sizeof(temp_ttl)) < 0)
	{
		perror("setsockopt (IP_TTL)"); //TODO: replace to remove forbidden function
		return (1);
	}
	for (int try = 0; try < exec->context->tries_per_hop; try++)
	{
		if (single_try(&is_first, &is_first_successful, exec))
			return (1);
	}
	printf("\n");
	return (0);
}

/*
 * Main function for traceroute execution
*/
int	execute_traceroute(t_traceroute_exec *exec)
{
	exec->dest_reached = 0;
	exec->current_ttl = exec->context->initial_ttl;
	exec->hop_index = 1;
	exec->seq_num = 1;
	exec->packet_size = DEFAULT_PACKET_SIZE;

	while (!exec->dest_reached && exec->hop_index <= exec->context->max_hop_count)
	{	
		if (single_hop(exec))
			return (1);
		exec->current_ttl++;
		exec->hop_index++;
		exec->seq_num++;
	}
	return (0);
}