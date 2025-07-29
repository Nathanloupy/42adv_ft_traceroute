#include "commons.h"

/*
 * Helper function to extract and validate ICMP packet from IP response
 */
static int	parse_icmp_response(t_traceroute_exec *exec, char *buffer, size_t bytes_received)
{
	struct iphdr		*ip_hdr;
	struct icmphdr		*icmp_hdr;
	struct icmphdr		*original_icmp;
	int					ip_hdr_len;

	if (bytes_received < sizeof(struct iphdr))
		return (FT_TRACEROUTE_NO_REPLY);

	ip_hdr = (struct iphdr *)buffer;
	ip_hdr_len = ip_hdr->ihl * 4;

	if (bytes_received < ip_hdr_len + sizeof(struct icmphdr))
		return (FT_TRACEROUTE_NO_REPLY);

	icmp_hdr = (struct icmphdr *)(buffer + ip_hdr_len);

	if (icmp_hdr->type == ICMP_TIME_EXCEEDED)
	{
		if (bytes_received < ip_hdr_len + sizeof(struct icmphdr) + sizeof(struct iphdr) + sizeof(struct icmphdr))
			return (FT_TRACEROUTE_NO_REPLY);

		struct iphdr *original_ip = (struct iphdr *)(buffer + ip_hdr_len + sizeof(struct icmphdr));
		int original_ip_len = original_ip->ihl * 4;
		original_icmp = (struct icmphdr *)((char *)original_ip + original_ip_len);

		if (original_icmp->un.echo.id != exec->pid || original_icmp->un.echo.sequence != exec->seq_num)
			return (FT_TRACEROUTE_NO_REPLY);

		exec->hop_addr.sin_addr.s_addr = ip_hdr->saddr;
		return (FT_TRACEROUTE_OK);
	}
	else if (icmp_hdr->type == ICMP_ECHOREPLY)
	{
		if (icmp_hdr->un.echo.id != exec->pid || icmp_hdr->un.echo.sequence != exec->seq_num)
			return (FT_TRACEROUTE_NO_REPLY);

		exec->hop_addr.sin_addr.s_addr = ip_hdr->saddr;
		exec->dest_reached = 1;
		return (FT_TRACEROUTE_OK);
	}
	return (FT_TRACEROUTE_NO_REPLY);
}

/*
 * Processes a received packet
 */
int	process_received_packet(t_traceroute_exec *exec)
{
	ssize_t		bytes_received;
	socklen_t	addr_len;
	char		buffer[DEFAULT_BUFFER_SIZE];

	addr_len = sizeof(exec->hop_addr);
	bytes_received = recvfrom(exec->socket_fd, buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr*)&exec->hop_addr, &addr_len);
	get_timestamp(&exec->recv_time);
	exec->response_time_ms = get_elapsedtime_ms(&exec->send_time, &exec->recv_time);
	if (bytes_received < 0)
	{
		perror("recvfrom"); //TODO: replace to remove forbidden function
		return (FT_TRACEROUTE_ERROR);
	}
	else if (!bytes_received)
		return (FT_TRACEROUTE_NO_REPLY);
	return (parse_icmp_response(exec, buffer, (size_t)bytes_received));
}