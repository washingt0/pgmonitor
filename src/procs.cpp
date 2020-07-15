#include <stdio.h>
#include <unistd.h>
#include "procs.hpp"
#include <stdlib.h>
#include <math.h>

unsigned long int get_mem_usage(int *pid) {
    FILE *f;
    char path[257];
    unsigned long int usage;

    sprintf(path, "/proc/%d/statm", *pid);

    f = fopen(path, "r");

    if (f == NULL) {
        return FILE_ERROR;
    }

    fscanf(f, "%lu", &usage);
    fclose(f);

    return usage * getpagesize();
}

int get_pid_cpu_stat(int *pid, long unsigned int *total_time, long long unsigned int *start_time) {
    char path[257];
    FILE *f;
    long unsigned int utime, stime, cutime, cstime;

    sprintf(path, "/proc/%d/stat", *pid);

    f = fopen(path, "r");
    if (f == NULL) {
        return FILE_ERROR;
    }

    fscanf(f, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %lu %lu %*d %*d %*d %*d %llu", &utime, &stime, &cutime, &cstime, start_time);

	*total_time = utime + stime + cutime + cstime;

	return 1;
}

long int get_uptime() {
	FILE *f;
	long int uptime;

	f = fopen("/proc/uptime", "r");

	if (f == NULL) {
		return FILE_ERROR;
	}

	fscanf(f, "%ld", &uptime);
	fclose(f);

	return uptime;
}

int get_cpu_usage(int *pid) {
	long unsigned int total_time;
	long long unsigned int start_time;

	int res = get_pid_cpu_stat(pid, &total_time, &start_time);

	if (res == -1) {
		return -1;
	}

	long long unsigned int seconds = get_uptime() - (start_time/sysconf(_SC_CLK_TCK));

	printf("%lf\n", ((double) ((double) total_time/sysconf(_SC_CLK_TCK)) / seconds));

    return round(100 *((double) ((double) total_time/sysconf(_SC_CLK_TCK)) / seconds));
}
