# Benchmark Library

## Overview

The Benchmark Library provides a framework for measuring the performance of functions in C. It allows you to run tests, gather timing data, and calculate average execution times along with deviations. This is useful for performance analysis and optimization of code.

## Features

- **Warmup Phase**: Prepares the system for accurate timing by running initial tests.
- **Testing Phase**: Measures the execution time of the specified function multiple times.
- **Reporting**: Outputs detailed results including total time, number of tests, average time per test, and relative deviation.
- **Configurable**: Customize warmup time, maximum test duration, and allowable deviation.

## Usage

### Setting Up the Benchmark

1. **Include the Library**: Include the benchmark header in your source file.

    ```c
    #include "benchmark.h"
    ```

2. **Define a Test Function**: Create the function you want to benchmark.

    ```c
    void my_test_function(state_t state) {
        DoNotOptimize({variable});
        // Function implementation
    }
    ```


    Use DoNotOptimize() to guarantee accurate measurements.

3. **Register the Test Function**:

    ```c
    benchmark_func(my_test_function);
    ```

4. **Configure Parameters** (Optional):

    ```c
    set_min_warmup_time(2.0);  // Set warmup time in seconds
    set_max_testing_time(5.0); // Set maximum testing time in seconds
    set_epsilon(0.01);         // Set allowable deviation
    ```

5. **Run the Benchmark**:

    ```c
    run_benchmark();
    ```

### Example

Here’s a simple example demonstrating how to use the Benchmark Library:

```c
#include <stdio.h>
#include "benchmark.h"

void my_test_function(state_t state) {
    // Simulated work
    for (size_t i = 0; i < 1000000; i++);
}

int main() {
    benchmark_func(my_test_function); // Register the test function
    set_min_warmup_time(1.0); // Set warmup time to 1 second
    set_max_testing_time(10.0); // Set maximum testing time to 10 seconds
    run_benchmark(); // Execute the benchmarking
    return 0;
}
```

## Results

The benchmark will output results to `stdout` in the following format:

### Warmup Results
- **Total Warmup Time**: The total duration spent in the warmup phase, measured in seconds.
- **Number of Warmup Tests**: The total number of test iterations performed during the warmup phase.

### Testing Results
- **Total Test Time**: The cumulative time taken for all test iterations, measured in seconds.
- **Number of Tests**: The total count of tests executed during the benchmarking.
- **Average Time per Test**: The average duration of each test iteration, calculated as `Total Test Time / Number of Tests`.
- **Average Relative Deviation**: The average relative deviation of the execution times, expressed as a percentage. This provides insight into the consistency of the test execution times.

### Example Output
Here is an example of how the results might appear in the console:

[Warmup results]:

    [time]: 2.500000

    [amount of tests]: 5

[Testing results]:

    [time]: 10.000000

    [amount of tests]: 100

    [average time per test]: 0.100000

    [average relative deviation of the group of 100]: 2.50 %

