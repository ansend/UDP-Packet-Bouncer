 /** @file posix_periodic.h
 *  @brief Declarations to make periodic tasks and time operation more handy
 *
 *  @author Thomas Krueger
 *  @bug No known bugs.
 */
#ifndef POSIX_PERIODIC_H_V70N
#define POSIX_PERIODIC_H_V70N

#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
struct periodic_info
{
	int timer_fd;
	unsigned long long wakeups_missed;
};

timespec Calc_Time_Diff(timespec start, timespec end);

int make_periodic (unsigned int period, struct periodic_info *info);

void wait_period (struct periodic_info *info);

double timespec2double (timespec time_to_convert);

#endif 