#define FILE_ERROR -1;
#define OPEN_DIR_ERROR "open dir failed"

int get_cpu_usage(int *pid);

unsigned long long int get_mem_usage(const int *pid);

void walk_procs(const int *cpu, const int *mem);
