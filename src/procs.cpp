#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "procs.hpp"
#include <cmath>
#include <dirent.h>

unsigned long long int get_mem_usage(const int *pid) {
    FILE *f;
    char path[257];
    unsigned long long int usage;

    sprintf(path, "/proc/%d/statm", *pid);

    f = fopen(path, "r");

    if (f == nullptr) {
        return FILE_ERROR;
    }

    fscanf(f, "%llu", &usage);
    fclose(f);

    return usage * (unsigned long long int)getpagesize();
}

int get_pid_cpu_stat(const int *pid, long unsigned int *total_time, long long unsigned int *start_time) {
    char path[257];
    FILE *f;
    long unsigned int utime, stime, cutime, cstime;

    sprintf(path, "/proc/%d/stat", *pid);

    f = fopen(path, "r");
    if (f == nullptr) {
        return FILE_ERROR;
    }

    fscanf(f, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %lu %lu %*d %*d %*d %*d %llu", &utime, &stime, &cutime, &cstime, start_time);
    fclose(f);

	*total_time = utime + stime + cutime + cstime;

	return 1;
}

long long int get_uptime() {
	FILE *f;
	long long int uptime;

	f = fopen("/proc/uptime", "r");

	if (f == nullptr) {
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

    return (int) round(100 *((double) ((double)total_time/(double)sysconf(_SC_CLK_TCK)) / (double) seconds));
}

int walk_procs(const int *cpu, const int *mem) {
    DIR *d;
    dirent *entry;

    d = opendir("/proc");
    if (d == nullptr) {
        return OPEN_DIR_ERROR;
    }

    pid_t _pid, _ppid;
    _pid = getpid();
    _ppid = getppid();

    while (true) {
        entry = readdir(d);
        if (entry == nullptr) break;
        if (entry->d_type != DT_DIR) continue;
        if (entry->d_name[0] < 48 || entry->d_name[0] > 57 ) continue;

        int pid = (int) strtol(&entry->d_name[0], nullptr, 10);

        if (pid == _pid || pid == _ppid) continue;

        int cpu_u, mem_u;

        cpu_u = get_cpu_usage(&pid);
        mem_u = (int) (get_mem_usage(&pid) / 1024 / 1024);

        if (cpu_u < *cpu && mem_u < *mem) continue;

        printf("PID: %5d\tCPU: %3d%%\tMEM: %6d MB\n", pid, cpu_u, mem_u);
    }

    closedir(d);

    return 0;
}
