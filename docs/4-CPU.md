##  GET CPU TIMES
The `get_cpu_times` function is designed to read and retrieve CPU time statistics from the Linux `/proc/stat` file. The function returns a `std::vector<size_t>` containing the CPU times for various states.

Here's a step-by-step explanation of the code with comments:

```cpp
std::vector<size_t> get_cpu_times() {
    // Open the /proc/stat file for reading
    std::ifstream proc_stat("/proc/stat");

    // Ignore the first 5 characters in the file, which are the "cpu" string
    // The first line in /proc/stat contains overall CPU time information, not specific to a single CPU core
    proc_stat.ignore(5, ' ');

    // Create a vector to store the CPU time values
    std::vector<size_t> times;

    // Read the CPU time values from the file and store them in the vector
    // The CPU time values are separated by spaces in the /proc/stat file
    for (size_t time; proc_stat >> time; times.push_back(time))
        ; // Empty loop body, the loop iterates until there's no more data to read

    // Return the vector containing the CPU time values
    return times;
}
```

Here's a summary of what the code does:

1. The function first opens the `/proc/stat` file for reading using `std::ifstream`. The `/proc/stat` file provides system statistics, including overall CPU time information.

2. The function then ignores the first 5 characters in the file, which are the string "cpu". This is done because the first line in `/proc/stat` contains overall CPU time information, not specific to a single CPU core.

3. It creates an empty vector `times` to store the CPU time values.

4. The function uses a `for` loop to read the CPU time values from the file one by one. The values are separated by spaces in the `/proc/stat` file.

5. For each value read from the file, it is converted to a `size_t` type (unsigned integer) and added to the `times` vector using `times.push_back(time)`.

6. The loop continues until there is no more data to read from the file. The loop keeps reading values from the file and storing them in the `times` vector until the end of the file is reached.

7. Finally, the function returns the `times` vector containing the CPU time values.

Overall, this function provides a way to extract CPU time statistics from the `/proc/stat` file, which can be useful for various performance monitoring and analysis tasks on a Linux system. The returned vector contains values representing the CPU time for different states (e.g., user, nice, system, idle, etc.), and these values can be used to calculate CPU utilization and other performance metrics.

The `get_cpu_times` function has been modified from the previous version. Now, it takes two output parameters, `idle_time` and `total_time`, and returns a boolean value indicating whether it successfully retrieved the CPU times or not.

Here's a step-by-step explanation of the updated code with comments:

```cpp
bool get_cpu_times(size_t &idle_time, size_t &total_time) {
    // Get the CPU times using the previously defined function get_cpu_times()
    const std::vector<size_t> cpu_times = get_cpu_times();

    // Check if the CPU times vector has at least four elements (user, nice, system, idle)
    if (cpu_times.size() < 4)
        return false;

    // The third element in the vector (index 3) corresponds to the idle time
    idle_time = cpu_times[3];

    // Calculate the total CPU time by summing up all the elements in the vector
    total_time = std::accumulate(cpu_times.begin(), cpu_times.end(), 0);

    // Return true to indicate successful retrieval of CPU times
    return true;
}
```

Here's a summary of what the updated code does:

1. The function first calls the `get_cpu_times()` function, which retrieves the CPU time statistics from the `/proc/stat` file and returns them as a `std::vector<size_t>`.

2. It checks if the size of the `cpu_times` vector is less than 4. The vector should have at least four elements: user, nice, system, and idle CPU times. If it doesn't, it means that the CPU times might not have been properly retrieved, so the function returns `false`.

3. If the `cpu_times` vector has at least four elements, it proceeds to extract the idle time from the vector. The idle time is the third element of the vector (index 3).

4. The function then calculates the total CPU time by summing up all the elements in the `cpu_times` vector. It uses `std::accumulate` to add up the values in the vector and stores the result in the `total_time` variable.

5. Finally, the function returns `true` to indicate that it successfully retrieved the CPU times and assigned the `idle_time` and `total_time` variables.

This updated function is useful for getting the current idle time and total CPU time, which can be used to calculate CPU utilization and perform other performance monitoring tasks on a Linux system.

##  UTILIZATION
The `Processor::Utilization()` function is a member function of the `Processor` class. Its purpose is to calculate the current CPU utilization and update a log of recent CPU utilization values.

Here's a step-by-step explanation of the code with comments:

```cpp
float Processor::Utilization() {
    // Initialize variables to store the current idle time and total time
    size_t idle_time = 0, total_time = 0;

    // Call the get_cpu_times function to retrieve the current CPU times
    get_cpu_times(idle_time, total_time);

    // Calculate the delta (difference) in idle time and total time since the last call to Utilization()
    const float idle_time_delta = idle_time - Processor::CPU_Previous_Idle;
    const float total_time_delta = total_time - Processor::CPU_Previous_Total;

    // Calculate the current CPU utilization as (1 - idle_time_delta / total_time_delta)
    const float utilization = (1.0 - idle_time_delta / total_time_delta);

    // Update the previous idle time and total time for the next call to Utilization()
    Processor::CPU_Previous_Idle = idle_time;
    Processor::CPU_Previous_Total = total_time;

    // Shift the elements in the Cpu_Usage_Log array to the left by one position
    // This effectively discards the oldest utilization value and makes space for the new one
    for (int j = 0; j < 29; j++) {
        Processor::Cpu_Usage_Log[j] = Processor::Cpu_Usage_Log[j + 1];
    }

    // Store the current utilization (in percentage) in the last element of the Cpu_Usage_Log array
    Processor::Cpu_Usage_Log[29] = utilization * 100;

    // Return the current CPU utilization
    return utilization;
}
```

Here's a summary of what the code does:

1. The function first declares two variables, `idle_time` and `total_time`, to store the current idle time and total time of the CPU.

2. It calls the `get_cpu_times()` function to retrieve the current CPU times. The `get_cpu_times()` function was previously defined to obtain CPU times from the `/proc/stat` file.

3. The function calculates the change (delta) in idle time and total time since the last call to `Utilization()` and stores these deltas in the variables `idle_time_delta` and `total_time_delta`.

4. It calculates the current CPU utilization as (1 - `idle_time_delta` / `total_time_delta`). This formula calculates the percentage of time the CPU has been busy executing tasks.

5. The function updates the previous idle time and total time (`Processor::CPU_Previous_Idle` and `Processor::CPU_Previous_Total`) with the current values for the next call to `Utilization()`.

6. The function then shifts the elements in the `Cpu_Usage_Log` array to the left by one position, effectively discarding the oldest utilization value and making space for the new one.

7. The current utilization value (in percentage) is stored in the last element of the `Cpu_Usage_Log` array, at index 29.

8. Finally, the function returns the calculated CPU utilization.

This function can be used to monitor the CPU utilization over time and log the recent utilization values in the `Cpu_Usage_Log` array, which can be useful for performance monitoring and analysis tasks. The `Cpu_Usage_Log` array stores the most recent 30 utilization values (in percentage) and can be accessed later to visualize or analyze CPU usage trends.

##  CORE COUNT
The `Processor::CoreCount()` function is a member function of the `Processor` class. Its purpose is to retrieve the number of CPU cores (logical cores) on the system and store the count in the `Processor::Cpu_Count` variable. It then returns the number of CPU cores.

Here's a step-by-step explanation of the code with comments:

```cpp
int Processor::CoreCount() {
    // Declare a string variable to store the CPU count value
    std::string Cpucount;

    // Declare a vector of strings to store the words read from /proc/cpuinfo
    std::vector<std::string> words;

    // Specify the path to the /proc/cpuinfo file
    std::string path = "/proc/cpuinfo";

    // Open the /proc/cpuinfo file for reading
    std::ifstream proc_Sys(path.c_str(), std::ifstream::in);
    std::string str;

    // Read the contents of /proc/cpuinfo and store the words in the 'words' vector
    while (proc_Sys >> str) {
        words.push_back(str);
    }

    // Close the /proc/cpuinfo file
    proc_Sys.close();

    // Loop through the words vector to find the CPU count value
    for (size_t i = 0; i < words.size(); i++) {
        if (words[i] == "siblings") {
            // The CPU count value is stored in the word after "siblings" in the file
            Cpucount = words[i + 2];
        }
    }

    // Convert the string representation of the CPU count to an integer
    Processor::Cpu_Count = atoi(Cpucount.c_str());

    // Return the number of CPU cores
    return Processor::Cpu_Count;
}
```

Here's a summary of what the code does:

1. The function first declares a string variable `Cpucount` to store the CPU count value read from the `/proc/cpuinfo` file.

2. It declares a vector of strings named `words` to store the words read from the `/proc/cpuinfo` file. Each word represents a piece of information in the file.

3. The function specifies the path to the `/proc/cpuinfo` file.

4. It opens the `/proc/cpuinfo` file for reading using `std::ifstream`.

5. The function reads the contents of the `/proc/cpuinfo` file word by word and stores each word in the `words` vector.

6. After reading the file, it closes the `/proc/cpuinfo` file.

7. The function then loops through the `words` vector to find the CPU count value. It searches for the word "siblings" in the vector and retrieves the value stored two words after "siblings."

8. It stores the CPU count value as a string in the `Cpucount` variable.

9. The function converts the string representation of the CPU count to an integer using `atoi` function and stores the result in the `Processor::Cpu_Count` variable.

10. Finally, the function returns the number of CPU cores, which is the value of `Processor::Cpu_Count`.

This function allows you to determine the number of CPU cores (logical cores) available on the system by reading the `/proc/cpuinfo` file, which can be useful for various system monitoring and optimization tasks.

##  GET CPU TYPE
The `Processor::GetCPUType()` function is a member function of the `Processor` class. Its purpose is to retrieve the CPU model name from the `/proc/cpuinfo` file and return it as a string.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Processor::GetCPUType() {
    // Open the /proc/cpuinfo file for reading
    FILE* fp = fopen("/proc/cpuinfo", "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        // Return an empty string if the file cannot be opened
        return "";
    }

    // Declare a char array to store each line read from the file
    char line[256];

    // Read each line from the file using fgets
    while (fgets(line, sizeof(line), fp)) {
        // Check if the line contains the string "model name"
        if (strncmp(line, "model name", 10) == 0) {
            // Find the colon (:) character in the line
            char* colon = strchr(line, ':');

            // If a colon is found, it indicates the start of the CPU model name
            if (colon) {
                // Return the CPU model name (starting from the colon)
                return colon;
            }

            // If a colon is not found, break out of the loop
            break;
        }
    }

    // Close the /proc/cpuinfo file
    fclose(fp);

    // Return an empty string if the CPU model name is not found
    return "";
}
```

Here's a summary of what the code does:

1. The function first attempts to open the `/proc/cpuinfo` file for reading using the `fopen()` function. It checks if the file was opened successfully by checking if the file pointer `fp` is `NULL`. If the file cannot be opened (e.g., due to permission issues or file not found), the function returns an empty string to indicate an error.

2. Inside the loop, the function reads each line from the `/proc/cpuinfo` file using the `fgets()` function, and each line is stored in the `line` character array.

3. The function checks if the current line starts with the string "model name" (using `strncmp()` to compare the first 10 characters). If this condition is met, it means the line contains the CPU model name.

4. It then finds the colon (':') character in the line using `strchr()`. The colon indicates the start of the CPU model name.

5. If a colon is found (i.e., `colon` is not `NULL`), the function returns the CPU model name (starting from the colon) as a string.

6. If a colon is not found (e.g., malformed line), the loop breaks out, and the function proceeds to close the `/proc/cpuinfo` file using `fclose()`.

7. If the CPU model name is not found or any error occurs during file reading, the function returns an empty string (`""`) to indicate that the CPU model name could not be retrieved.

This function provides a simple way to retrieve the CPU model name from the `/proc/cpuinfo` file, which can be useful for identifying the CPU type and model in a Linux system.

##  CPU MEAN 1M - 5M
The `Processor::CpuMean1m()` and `Processor::CpuMean5m()` functions are member functions of the `Processor` class. These functions retrieve the average CPU load over the last 1 minute and 5 minutes, respectively, from the system using the `sysinfo()` function.

Here's a step-by-step explanation of the code with comments:

```cpp
float Processor::CpuMean1m() {
    // Declare a struct to store system information
    struct sysinfo info;

    // Call sysinfo() to get system information and store it in the 'info' struct
    sysinfo(&info);

    // Calculate the average CPU load over the last 1 minute
    // The 1-minute load average is stored in the 'loads' array at index 0
    float avgload = info.loads[0] / 100000.0;

    // Return the average CPU load over the last 1 minute
    return avgload;
}

float Processor::CpuMean5m() {
    // Declare a struct to store system information
    struct sysinfo info;

    // Call sysinfo() to get system information and store it in the 'info' struct
    sysinfo(&info);

    // Calculate the average CPU load over the last 5 minutes
    // The 5-minute load average is stored in the 'loads' array at index 1
    float avgload = info.loads[1] / 100000.0;

    // Return the average CPU load over the last 5 minutes
    return avgload;
}
```

Here's a summary of what the code does:

1. Both functions start by declaring a struct `info` of type `sysinfo`. This struct is used to store various system information, including the CPU load averages.

2. The functions call the `sysinfo()` function, passing the `info` struct as an argument. This function retrieves system information and populates the `info` struct with the relevant data.

3. In `Processor::CpuMean1m()`, the function calculates the average CPU load over the last 1 minute. The 1-minute load average is stored in the `loads` array at index 0 of the `info` struct. The calculated average load is then divided by 100000.0 to convert it to a floating-point value. This is because the load values in the `info` struct are represented in fixed-point format, and dividing by 100000.0 converts them to floating-point values representing the actual load averages.

4. In `Processor::CpuMean5m()`, the function calculates the average CPU load over the last 5 minutes. The 5-minute load average is stored in the `loads` array at index 1 of the `info` struct. The calculated average load is also divided by 100000.0 to convert it to a floating-point value.

5. Both functions return the calculated average CPU load over the specified time intervals (1 minute for `Processor::CpuMean1m()` and 5 minutes for `Processor::CpuMean5m()`).

The `sysinfo()` function is part of the Linux C library and provides a way to retrieve various system-related information, including CPU load averages, uptime, memory usage, and more. These functions use `sysinfo()` to obtain the system load averages and convert them to floating-point values for use in the application. The CPU load averages can be helpful for monitoring system performance and resource utilization over time.

##  PROCESSOR
The `Processor::Processor()` constructor is a member function of the `Processor` class. This constructor is automatically called when a new `Processor` object is created. Its purpose is to initialize the `Cpu_Usage_Log` array to all zeros.

Here's a step-by-step explanation of the code with comments:

```cpp
Processor::Processor(void) {
    // This is the constructor for the Processor class.
    // It is automatically called when a new Processor object is created.

    // The constructor initializes the Cpu_Usage_Log array to all zeros.

    // Loop through each element (usage) in the Cpu_Usage_Log array
    for (auto &usage : Cpu_Usage_Log) {
        // Set each element to zero
        usage = 0;
    }
}
```

Here's a summary of what the code does:

1. The `Processor::Processor(void)` constructor is defined for the `Processor` class. The `void` in the parentheses indicates that the constructor takes no arguments.

2. When a new `Processor` object is created, the constructor is automatically called.

3. Inside the constructor, there is a loop that iterates through each element (usage) in the `Cpu_Usage_Log` array.

4. For each element, it sets the value to zero. This effectively initializes the entire `Cpu_Usage_Log` array to all zeros.

The purpose of initializing `Cpu_Usage_Log` to zeros in the constructor is to ensure that all elements of the array start with a clean slate before the application starts using them. This way, any previous data in the array is cleared, and the array is ready to store new CPU utilization log data when needed. Initializing variables and arrays to known values is a good practice to avoid potential issues with uninitialized data and ensure consistent behavior in the program.