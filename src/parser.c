#include "commons.h"

/* lpyp options definition */
static t_lpyp_options	ft_traceroute_options[] = {
	{'f', "first-hop", 'f', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "set initial hop distance, i.e. time-to-live, to N", "N"},
	{'m', "max-hop", 'm', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "set maximal hop count to N (default: 64)", "N"},
	{'q', "tries", 'q', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "send N probe packets per hop (default: 3)", "N"},
	{'r', "resolve-hostnames", 'r', LPYP_NO_ARG, "resolve hostnames", NULL},
	{'w', "wait", 'w', LPYP_REQUIRED_ARG | LPYP_DENY_DUPLICATE, "wait N seconds for response (default: 3)", "N"},
	{'h', "help", 'h', LPYP_NO_ARG, "give this help list", NULL},
	{0, "usage", 'u', LPYP_NO_ARG, "give a short usage message", NULL},
	{0, NULL, 0, 0, NULL, NULL} /* Sentinel */
};


/*
 * lpyp parser callback function
 */
static int	ft_traceroute_parser(unsigned int key, void *data, char *argument)
{
	t_traceroute_context	*context = (t_traceroute_context *)data;

	switch (key)
	{
		case 'f':
			context->initial_ttl = ft_atoi(argument);
			if (context->initial_ttl <= 0)
			{
				fprintf(stderr, "ft_traceroute: invalid initial hop distance value: %s\n", argument);
				return (1);
			}
			break;
		case 'm':
			context->max_ttl = ft_atoi(argument);
			if (context->max_ttl <= 0)
			{
				fprintf(stderr, "ft_traceroute: invalid maximal hop count value: %s\n", argument);
				return (1);
			}
			break;
		case 'q':
			context->tries_per_hop = ft_atoi(argument);
			if (context->tries_per_hop <= 0)
			{
				fprintf(stderr, "ft_traceroute: invalid probe packets per hop value: %s\n", argument);
				return (1);
			}
			break;
		case 'r':
			context->flags |= FLAG_RESOLVE_HOSTNAMES;
			break;
		case 'w':
			context->wait_response = ft_atoi(argument);
			if (context->wait_response <= 0)
			{
				fprintf(stderr, "ft_traceroute: invalid wait for response value: %s\n", argument);
				return (1);
			}
			break;
		case 'h':
			lpyp_help(ft_traceroute_options, "ft_traceroute", "Print the route packets trace to network host.");
			exit(0);
		case 'u':
			lpyp_usage(ft_traceroute_options, "ft_traceroute");
			exit(0);
		case LPYP_KEY_ARG:
			if (context->dest_host == NULL)
				context->dest_host = argument;
			else
			{
				fprintf(stderr, "ft_traceroute: too many arguments\n");
				return (1);
			}
			break;
		case LPYP_KEY_END:
			if (context->dest_host == NULL)
			{
				fprintf(stderr, "ft_traceroute: missing host operand\n");
				fprintf(stderr, "Try 'ft_traceroute --help' or 'ft_traceroute --usage' for more information.\n");
				return (1);
			}
			break;
		case LPYP_KEY_UNKNOWN:
			fprintf(stderr, "Try 'ft_traceroute --help' for more information.\n");
			return (1);
	}
	return (0);
}

/*
 * Initialize context with default values
 */
static void init_context_defaults(t_traceroute_context *context)
{
	context->initial_ttl = DEFAULT_INITIAL_TTL;
	context->max_ttl = DEFAULT_MAX_TTL;
	context->tries_per_hop = DEFAULT_TRIES_PER_HOP;
	context->wait_response = DEFAULT_WAIT_RESPONSE;
	context->flags = 0;
	context->dest_host = NULL;
}

/*
 * Parse command line arguments using lpyp
 */
int	parse_arguments(int argc, char *argv[], t_traceroute_context *context)
{
	init_context_defaults(context);
	return (lpyp_parse(context, argc, argv, ft_traceroute_options, ft_traceroute_parser));
}