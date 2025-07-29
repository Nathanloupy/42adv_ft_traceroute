#include "commons.h"

/*
 * Waits for a response with timeout using select
*/
int	wait_packet(t_traceroute_exec *exec)
{
	struct timeval	tv;
	int				ret;
	fd_set			rfds;
	int				packet_received = 0;
	long			timeout_ms = exec->context->wait_response * 1000;

	while (!packet_received)
	{
		if (calculate_timeout(&tv, timeout_ms, &exec->send_time) == 0)
			return (FT_TRACEROUTE_TIMEOUT);

		FD_ZERO(&rfds);
		FD_SET(exec->socket_fd, &rfds);

		ret = select(exec->socket_fd + 1, &rfds, NULL, NULL, &tv);
		if (ret < 0)
		{
			fprintf(stderr, "ft_traceroute: select: %s\n", strerror(errno));
			return (FT_TRACEROUTE_ERROR);
		}
		else if (!ret)
			return (FT_TRACEROUTE_TIMEOUT);

		ret = process_received_packet(exec);
		switch (ret)
		{
			case FT_TRACEROUTE_OK:
				return (FT_TRACEROUTE_OK);
			case FT_TRACEROUTE_ERROR:
				return (FT_TRACEROUTE_ERROR);
			default:
				continue;
		}
	}
	return (FT_TRACEROUTE_OK);
}