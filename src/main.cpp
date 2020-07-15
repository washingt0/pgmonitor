#include <iostream>
#include "procs.hpp"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Missing PID" << std::endl;
		return 1;
	}

    int a = atoi(argv[1]);
    std::cout << get_mem_usage(&a) / 1024 / 1024 << " MB"<< std::endl;
	std::cout << get_cpu_usage(&a) << " %" <<  std::endl;

    return 0;
}
