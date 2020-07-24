#include <iostream>
#include "procs.hpp"
#include <stdexcept>

int main(int argc, char *argv[]) {
    int cpu, mem;
    cpu = 30;
    mem = 1000;

    if (argc == 1) {
        printf("Using default values: CPU -> %d%%\tMEM -> %d MB\n", cpu, mem);
        printf("To define yours use %s <CPU> <MEM>\n\n", argv[0]);
    }

    if (argc > 2) {
        mem = (int) strtol(&argv[2][0], nullptr, 10);
    }

    if (argc > 1) {
        cpu = (int) strtol(&argv[1][0], nullptr, 10);
    }

    try {
        walk_procs(&cpu, &mem);
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "internal exception" << std::endl;
    }

    return 0;
}
