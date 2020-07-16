#include <iostream>
#include "procs.hpp"

int main(int argc, char *argv[]) {
    int cpu, mem;
    cpu = 30;
    mem = 1000;

    if (argc == 1) {
        printf("Using default values: CPU -> %d%%\tMEM -> %d MB\n", cpu, mem);
        printf("To define yours use %s <CPU> <MEM>\n\n", argv[0]);
    }

    if (argc > 3) {
        mem = atoi(argv[2]);
    }

    if (argc > 2) {
        cpu = atoi(argv[1]);
    }


    walk_procs(&cpu, &mem);

    return 0;
}
