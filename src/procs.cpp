#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "procs.hpp"
#include <stdlib.h>
#include <math.h>
#include <dirent.h>

unsigned long long int get_mem_usage(int *pid) {
    FILE *f;
    char path[257];
    unsigned long long int usage;

    sprintf(path, "/proc/%d/statm", *pid);

    f = fopen(path, "r");

    if (f == NULL) {
        return FILE_ERROR;
    }

    fscanf(f, "%llu", &usage);
    fclose(f);

    return usage * (unsigned long long int)getpagesize();
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

long long int get_uptime() {
	FILE *f;
	long long int uptime;

	f = fopen("/proc/uptime", "r");

	if (f == NULL) {
		return FILE_ERROR;
	}

	fscanf(f, "%lld", &uptime);
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

	unsigned long long int seconds = get_uptime() - (start_time/sysconf(_SC_CLK_TCK));

    return round(100 *((double) (total_time/sysconf(_SC_CLK_TCK)) / seconds));
}

int walk_procs(int *cpu, int *mem) {
    DIR *d;
    dirent *entry;

    d = opendir("/proc");
    if (d == NULL) {
        return OPEN_DIR_ERROR;
    }

    pid_t _pid, _ppid;
    _pid = getpid();
    _ppid = getppid();

    while (1) {
        entry = readdir(d);
        if (entry == NULL) break;
        if (entry->d_type != DT_DIR) continue;
        if (entry->d_name[0] < 48 || entry->d_name[0] > 57 ) continue;

        int pid = atoi(entry->d_name);

        if (pid == _pid || pid == _ppid) continue;

        int cpu_u, mem_u;

        cpu_u = get_cpu_usage(&pid);
        mem_u = (get_mem_usage(&pid) / 1024 / 1024);

        if (cpu_u < *cpu && mem_u < *mem) continue;

        printf("PID: %5d\tCPU: %3d%%\tMEM: %6d MB\n", pid, cpu_u, mem_u);
    }


    return 0;
}
