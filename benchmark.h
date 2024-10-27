#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h>

#include "queue.h"

#define BENCHMARK(func) benchmark_func(func)

#define DoNotOptimize(value) asm volatile("" : "+m"(value) : : "memory")

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
    clock_t min_warmup_time;
    clock_t max_test_time;

    size_t iterations;
    double epsilon;

    test_func_t func;

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
    circ_buffer_t buffer;
    double average;
    size_t length;
} group_deviation_t;

benchmark_t* benchmark();
void run_benchmark();
void benchmark_func(test_func_t test_func);

void set_min_warmup_time(double seconds);
void set_epsilon(double epsilon);
void set_max_testing_time(double seconds);
void print_report();

#endif /* BENCHMARK_H */
