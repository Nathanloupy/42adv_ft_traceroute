#include "commons.h"

/*
 * Gets the current timestamp using gettimeofday
*/
void	get_timestamp(struct timeval *timestamp)
{
	gettimeofday(timestamp, NULL);
}

/*
 * Calculates elapsed time in milliseconds between two timestamps
*/
double	get_elapsedtime_ms(struct timeval *start, struct timeval *end)
{
	return ((end->tv_sec - start->tv_sec) * 1000 + (end->tv_usec - start->tv_usec) / 1000.0);
}

/*
 * Calculates the timeout for the select function
*/
int	calculate_timeout(struct timeval *tv, long timeout_ms, struct timeval *send_time)
{
	struct timeval	now;
	get_timestamp(&now);

	long	elapsed = (long)get_elapsedtime_ms(send_time, &now);
	long	remain = timeout_ms - elapsed;

	if (remain <= 0)
		return (0);

	tv->tv_sec = remain / 1000;
	tv->tv_usec = (remain % 1000) * 1000;
	return (1);
}
