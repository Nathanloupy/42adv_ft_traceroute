#include "commons.h"
#include <sys/socket.h>

/*
Steps to trace a route :
1 - parsing (done)
2 - DNS resolution to host_address (done)
3 - set socket (done)
4 - print header of traceroute (done)
5 - from 1 to max_hop_count (unless destination reached)
		* set socket ttl and prepare packet
		* from 1 to <tries_per_hop>
			. send packet
			. wait <wait_response> like in ping
			. print the hop ip if not already printed on the hop wait
*/

static void	safe_exit(t_traceroute_exec *exec)
{
	if (exec->socket_fd > 2)
	{
		close(exec->socket_fd);
		exec->socket_fd = -1;
	}
}

int	main(int argc, char *argv[])
{
	t_traceroute_context	context;
	t_traceroute_exec		exec;

	ft_memset(&exec, 0, sizeof(t_traceroute_exec));
	exec.context = &context;
	exec.pid = getpid() & 0xFFFF;

	if (parse_arguments(argc, argv, &context))
		return (1);
	if (initialize_icmp_socket(&exec))
		return (safe_exit(&exec), 1);
	if (print_header(&exec))
		return (safe_exit(&exec), 1);
	if (execute_traceroute(&exec))
		return (safe_exit(&exec), 1);
	return (safe_exit(&exec), 0);
}