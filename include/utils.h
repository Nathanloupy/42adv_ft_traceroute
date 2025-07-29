#pragma once

#include "commons.h"

/*	UTILS - TIME */
void	get_timestamp(struct timeval *timestamp);
double	get_elapsedtime_ms(struct timeval *start, struct timeval *end);
int     calculate_timeout(struct timeval *tv, long timeout_ms, struct timeval *send_time);

/*	UTILS - CHECKSUM */
unsigned short		checksum(void *buffer, int length);
