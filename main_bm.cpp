#include <stdio.h>
#include <string.h>
#include "benchmark.h"

void multiplication(state_t state) {
    int a = 0;
    int b = 10;

    while (state) {
        for (size_t i = 0; i < 100000000; i++) {
            memcpy(&a, &b, sizeof(int));
        }
        state = STOP;
    }
}

int main() {
    BENCHMARK(multiplication);

    run_benchmark();

    // double relative_deviation = benchmark()->testing_results.average_relative_deviation;
    // double average_time = benchmark()->testing_results.average_time;
    // printf("time of operation = %f\nrelative deviation = %f\ncnt = %lu\n",
    //         average_time, relative_deviation * 100, benchmark()->testing_results.tests_cnt);
}
