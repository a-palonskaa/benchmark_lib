#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "benchmark.h"

static void set_warmup_results(test_t* results);
static void set_begin_results(test_t* results);
static void set_testing_results(test_t* results);

static void initialize_test_info(test_t* test, state_t state);

static clock_t run_test(state_t state);
static void run_warmup();
static void begin_testing();
static void run_testing();

static group_deviation_t* group_deviation();
static void group_deviation_ctor();
static void group_deviation_dtor();
static void group_deviation_push(double* elm, state_t state);

static bool compare_doubles(double a, double b);
//============================================================================================================

const clock_t MIN_WARMUP_TIME = 10000000;
const clock_t MAX_TEST_TIME = 10000000;
const size_t CONTROL_GROUP_SIZE = 100;
const double EPSILON = 1e-2;
const double EPSILON_DOUBLE = 1e-9;

//============================================================================================================

benchmark_t* benchmark() {
    static benchmark_t test = {};
    return &test;
}

void benchmark_func(test_func_t test_func) {
    benchmark()->func = test_func;
}

void set_min_warmup_time(double seconds) {
    benchmark()->min_warmup_time = seconds * CLOCKS_PER_SEC;
}

void set_epsilon(double epsilon) {
    benchmark()->epsilon = epsilon;
}

void set_max_testing_time(double seconds) {
    benchmark()->max_test_time = seconds * CLOCKS_PER_SEC;
}

static void initialize_benchmark() {
    if (benchmark()->min_warmup_time == 0) {
        benchmark()->min_warmup_time = MIN_WARMUP_TIME;
    }

    if (compare_doubles(benchmark()->epsilon, 0)) {
        benchmark()->epsilon = EPSILON;
    }

    if (benchmark()->max_test_time == 0) {
        benchmark()->max_test_time = MAX_TEST_TIME;
    }
}

static void set_warmup_results(test_t* results) {
    benchmark()->warmup_results.time = results->total_time;
    benchmark()->warmup_results.tests_cnt = results->tests_cnt;
}

static void set_begin_results(test_t* results) {
    benchmark()->begin_results.time = results->total_time;
    benchmark()->begin_results.tests_cnt = results->tests_cnt;
}

static void set_testing_results(test_t* results) {
    benchmark()->testing_results.time = results->total_time;
    benchmark()->testing_results.tests_cnt = results->tests_cnt;
    benchmark()->testing_results.average_time = (double) results->total_time / results->tests_cnt;
    benchmark()->testing_results.average_relative_deviation = group_deviation()->average;
}

//============================================================================================================

void run_benchmark() {
    if (!benchmark()->func) {
        return;
    }

    initialize_benchmark();

    run_warmup();
    run_testing();

    print_report();
}

//============================================================================================================

static void initialize_test_info(test_t* test, state_t state) {
    test->state = state;
    test->tests_cnt = 0;
    test->total_time = 0;

    switch (state) {
        case WARMUP:
            test->set_time = benchmark()->min_warmup_time;
            test->set_iterations = 0;
            break;
        case BEGIN:
            test->set_time = 0;
            test->set_iterations = CONTROL_GROUP_SIZE;
            break;
        case KEEP:
            test->set_time = 0;
            test->set_iterations = 0;
            test->tests_cnt = benchmark()->begin_results.tests_cnt;
            test->total_time = benchmark()->begin_results.time;
            break;
        default:
            assert(0 && "Undefined state");
            break;
    }
}

//============================================================================================================

static clock_t run_test(state_t state) {
    clock_t start = clock();
    benchmark()->func(state);
    clock_t end = clock();

    return end - start;
}

static void run_warmup() {
    test_t warmup = {};
    initialize_test_info(&warmup, WARMUP);

    clock_t duration = 0;

    while (warmup.total_time < warmup.set_time) {
        duration = run_test(warmup.state);
        warmup.total_time += duration;
        warmup.tests_cnt++;
    }

    set_warmup_results(&warmup);
}

static void begin_testing() {
    test_t begin_tests = {};
    initialize_test_info(&begin_tests, BEGIN);

    double average = 0;
    double relative_deviation = 0;
    size_t begin_cnt = begin_tests.set_iterations;
    clock_t begin_time = 0;
    clock_t test_time = 0;

    for (size_t i = 0; i < begin_cnt; i++) {
        test_time = run_test(BEGIN);
        begin_time += test_time;
    }

    begin_tests.total_time = begin_time;
    begin_tests.tests_cnt = begin_cnt;

    while (begin_tests.tests_cnt++ < begin_tests.set_iterations + begin_cnt) {
        test_time = run_test(BEGIN);
        begin_tests.total_time += test_time;

        average = ((double) begin_tests.total_time) / begin_tests.tests_cnt;
        relative_deviation = fabs(test_time - average) / average;
        group_deviation_push(&relative_deviation, BEGIN);
    }

    begin_tests.tests_cnt--;
    set_begin_results(&begin_tests);
}

static void run_testing() {
    group_deviation_ctor();

    begin_testing();

    test_t main_tests = {};
    initialize_test_info(&main_tests, KEEP);

    double epsilon = benchmark()->epsilon;
    double relative_deviation = 0;
    double average = 0;
    clock_t max_test_time = benchmark()->max_test_time;
    clock_t test_time = 0;

    do {
        test_time = run_test(main_tests.state);
        main_tests.total_time += test_time;
        main_tests.tests_cnt++;

        average = (double) main_tests.total_time / main_tests.tests_cnt;
        relative_deviation = fabs(test_time - average) / average;
        group_deviation_push(&relative_deviation, KEEP);
    } while (group_deviation()->average > epsilon && main_tests.total_time < max_test_time);

    set_testing_results(&main_tests);

    group_deviation_dtor();
}

//============================================================================================================

static group_deviation_t* group_deviation() {
    static group_deviation_t group_deviation;
    return &group_deviation;
}

static void group_deviation_ctor() {
    group_deviation()->average = 0;
    group_deviation()->length = 0;

    cb_ctor(&group_deviation()->buffer, CONTROL_GROUP_SIZE, sizeof(double));
}

static void group_deviation_dtor() {
    group_deviation()->average = 0;
    group_deviation()->length  = 0;

    cb_dtor(&group_deviation()->buffer);
}

static void group_deviation_push(double* elm, state_t state) {
    assert(elm);
    size_t length = group_deviation()->length;

    if (state == BEGIN) {
        cb_push(&group_deviation()->buffer, elm);

        group_deviation()->average = (group_deviation()->average * length + *elm) / (length + 1);

        group_deviation()->length++;
    }
    else if (state == KEEP) {
        double popped_elm = 0;

        cb_pop(&group_deviation()->buffer, &popped_elm);
        cb_push(&group_deviation()->buffer, elm);

        group_deviation()->average = ((group_deviation()->average * length) - popped_elm + *elm) / length;
    }
}

//============================================================================================================

void print_report() {
    fprintf(stdout, "\n-------------Testing results--------------\n\n");

    fprintf(stdout, "\t[Testing time]: %f\n\t[Tests amount]: %zu\n"
                    "\t[Test avarage time]: %f\n\t[Avarage relative deviation]: = %2.2f%%\n\n",
                    (double) benchmark()->testing_results.time / CLOCKS_PER_SEC,
                    benchmark()->testing_results.tests_cnt,
                    benchmark()->testing_results.average_time,
                    benchmark()->testing_results.average_relative_deviation * 100);

    fprintf(stdout, "----------------Warmup-------------------\n\n");
    fprintf(stdout, "\t[Warmup time]: %f\n\t[Warmup tests amount]: %zu\n",
                    (double) benchmark()->warmup_results.time / CLOCKS_PER_SEC,
                    benchmark()->warmup_results.tests_cnt);

    fprintf(stdout, "\n----------------------------------------\n");
}


//============================================================================================================

static bool compare_doubles(double a, double b) {
    return fabs(a - b) < EPSILON_DOUBLE;
}
