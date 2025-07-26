#include "commons.h"

int main(int argc, char *argv[])
{
    t_traceroute_context	context;

	ft_memset(&context, 0, sizeof(context));

    if (parse_arguments(argc, argv, &context))
		return (1);
    return (0);
}