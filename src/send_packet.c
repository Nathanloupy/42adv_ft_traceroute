#include "commons.h"

/*
 * Sends an ICMP packet and records timestamp
*/
int	send_packet(t_traceroute_exec *exec)
{
	get_timestamp(&exec->send_time);
	return (sendto(exec->socket_fd, exec->packet, exec->packet_size, 0, (struct sockaddr*)&exec->dest_addr, sizeof(exec->dest_addr)));
}