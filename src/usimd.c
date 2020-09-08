#include "usimd.h"
int main() {
    /* Initialize CPU features */
    if (npy_cpu_init() < 0) {
        printf("error\n");
    }
}