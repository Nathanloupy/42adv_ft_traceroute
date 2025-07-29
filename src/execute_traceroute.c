#include "commons.h"

/*
TODO: remove later :
1 - send packet
2 - wait for reply
3 - if first try, print the hop_index
4 - if first success, print ip additionnal to timestamp
5 - on fail, print '*  '
*/

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
				printf("%3d  ", exec->hop_index);
				*is_first = 0;
			}
			if (*is_first_successful)
			{
				printf("%s  ", "<IP received>");
				*is_first_successful = 0;
			}
			printf("%.3fms  ", 9.9999999);
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

	if (setsockopt(exec->socket_fd, IPPROTO_IP, IP_TTL, &temp_ttl, sizeof(temp_ttl)) < 0)
		return (perror("setsockopt (IP_TTL)"), 1); //TODO: replace to remove forbidden function
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
	int	dest_reached = 0;
	exec->current_ttl = exec->context->initial_ttl;
	exec->hop_index = 1;
	exec->seq_num = 1;
	exec->packet_size = DEFAULT_PACKET_SIZE;

	while (!dest_reached && exec->hop_index <= exec->context->max_hop_count)
	{	
		if (single_hop(exec))
			return (1);
		exec->current_ttl++;
		exec->hop_index++;
		exec->seq_num++;
	}
	return (0);
}