#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h>

#include "circular_buffer.h"

#define BENCHMARK(func) benchmark_func(func)

typedef enum {
    STOP   = 0,
    WARMUP = 1,
    BEGIN  = 2,
    KEEP   = 3,
} state_t;

typedef struct {
    clock_t time;
    size_t tests_cnt;
} results_t;

typedef void (*test_func_t) (state_t state);

typedef struct {
    clock_t time;
    size_t tests_cnt;
    double average_time;
    double average_relative_deviation;
} testing_results_t;

typedef struct {
    test_func_t func;
    clock_t min_warmup_time;
    size_t iterations;
    clock_t max_test_time;
    double epsilon;
    results_t warmup_results;
    results_t begin_results;
    testing_results_t testing_results;
} benchmark_t;

typedef struct {
    state_t state;
    size_t tests_cnt;
    clock_t total_time;

    clock_t set_time;
    clock_t set_iterations;
} test_t;

typedef struct {
    circ_buffer_t* buffer;
    double average;
    size_t length;
} group_deviation_t;

benchmark_t* benchmark();
void run_benchmark();
void print_report();
void benchmark_func(test_func_t test_func);

void set_min_warmup_time(double seconds);
void set_epsilon(double epsilon);
void set_max_testing_time(double seconds);

#endif /* BENCHMARK_H */
