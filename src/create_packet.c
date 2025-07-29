#include "commons.h"

/*
 * Prepares and initializes an ICMP echo request packet with variable data size
*/
void	create_packet(struct icmphdr *icmp_hdr, char *packet, int seq_num, size_t packet_size, t_traceroute_exec *exec)
{
	size_t	total_size = sizeof(*icmp_hdr) + packet_size;
	
	ft_memset(icmp_hdr, 0, sizeof(*icmp_hdr));
	icmp_hdr->type = ICMP_ECHO;
	icmp_hdr->code = 0;
	icmp_hdr->un.echo.sequence = seq_num;
	icmp_hdr->un.echo.id = exec->pid;

	ft_memset(packet, 0, TRACEROUTE_MAX_DATALEN);
	for (size_t i = sizeof(*icmp_hdr); i < total_size; i++) 
		packet[i] = i % ICMP_DATA_PATTERN_MODULO;
	
	ft_memcpy(packet, icmp_hdr, sizeof(*icmp_hdr));
	icmp_hdr->checksum = 0;
	icmp_hdr->checksum = checksum(packet, total_size);
	ft_memcpy(packet, icmp_hdr, sizeof(*icmp_hdr));
}
