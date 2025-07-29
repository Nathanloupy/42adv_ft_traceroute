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
static int	single_try(int *is_first_successful, t_traceroute_exec *exec)
{
	int	result;

	if (send_packet(exec) < 0)
	{
		perror("sendto"); //TODO: replace to remove forbidden function
		return (1);
	}
	result = wait_packet(exec);

}

/*
 * Helper function to handle a single hop
*/
static int	single_hop(t_traceroute_exec *exec)
{
	int	is_first_successful = 1;
	int	temp_ttl = exec->current_ttl;

	if (setsockopt(exec->socket_fd, IPPROTO_IP, IP_TTL, &temp_ttl, sizeof(temp_ttl)) < 0)
		return (perror("setsockopt (IP_TTL)"), 1); //TODO: replace to remove forbidden function
	//TODO: hop_index
	for (int try = 0; try < exec->context->tries_per_hop; try++)
	{
		if (single_try(&is_first_successful, exec))
			return (printf("\n"), 1);
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

	while (!dest_reached && exec->current_ttl < exec->context->initial_ttl + exec->context->max_ttl)
	{	
		if (single_hop(exec))
			return (1);
		exec->current_ttl++;
		exec->hop_index++;
	}
	return (0);
}