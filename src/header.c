#include "commons.h"

/*
 * Prints the traceroute description header
 */
int print_header(t_traceroute_context *context)
{
	struct addrinfo		hints, *result;
	struct sockaddr_in	*addr_in;
	char				ip_str[INET_ADDRSTRLEN];
	
	if (inet_pton(AF_INET, context->dest_host, ip_str) > 0)
		printf("traceroute to %s (%s), %d hops max\n", context->dest_host, context->dest_host, context->max_ttl);
	else
	{
		ft_memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_RAW;
		if (getaddrinfo(context->dest_host, NULL, &hints, &result) != 0)
			return (fprintf(stderr, "ft_traceroute: unknown host %s\n", context->dest_host), 1);
		addr_in = (struct sockaddr_in *)result->ai_addr;
		if (inet_ntoa(addr_in->sin_addr) == NULL)
		{
			fprintf(stderr, "ft_traceroute: failed to convert address\n");
			freeaddrinfo(result);
			return (1);
		}
		printf("traceroute to %s (%s), %d hops max\n", context->dest_host, inet_ntoa(addr_in->sin_addr), context->max_ttl);
		freeaddrinfo(result);
	}
	return (0);
}