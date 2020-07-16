#define FILE_ERROR -1;
#define OPEN_DIR_ERROR -2;

int get_cpu_usage(int *pid);

unsigned long long int get_mem_usage(const int *pid);

int walk_procs(const int *cpu, const int *mem);
