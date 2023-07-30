##  System File Read
The given function `SysFileRead` is a C++ function that reads the contents of a system file located in the `/proc` directory and returns the data as a vector of strings.

Here's a step-by-step explanation of the function:

1. `std::vector<std::string> System::SysFileRead(std::string filename)`: This line defines the function `SysFileRead` that takes a `std::string` parameter called `filename` and returns a `std::vector<std::string>`.

2. `std::vector<std::string> words;`: This line declares a vector named `words` that will hold the individual words read from the system file.

3. `std::string path = "/proc/" + filename;`: This line creates a string `path` by concatenating the `/proc/` directory path with the `filename`. The `/proc/` directory in Linux contains virtual files that expose various system information.

4. `std::ifstream proc_Sys(path.c_str(), std::ifstream::in);`: This line creates an input file stream (`std::ifstream`) named `proc_Sys` and opens the file specified by the `path` string for reading. The `path.c_str()` converts the `path` string to a C-style character array (const char*) as required by the ifstream constructor.

5. `std::string str;`: This line declares a string variable `str` that will hold each word read from the file.

6. `while (proc_Sys >> str) { ... }`: This is a while loop that reads each word from the file using the `>>` operator of the input file stream `proc_Sys`. The loop continues until there are no more words to read.

7. `words.push_back(str);`: Inside the loop, each word read from the file is added to the `words` vector using the `push_back` function. This appends the word to the end of the vector.

8. `proc_Sys.close();`: After reading all the words from the file, the input file stream is closed using the `close` function.

9. `return words;`: Finally, the function returns the vector `words`, containing all the words read from the system file.

In summary, the `SysFileRead` function reads the contents of a system file located in the `/proc` directory and stores each word from the file into a vector of strings. This function can be used to extract data from specific system files exposed by the Linux virtual file system under `/proc`.

```cpp
// This function reads the contents of a system file located in the /proc directory and returns the data as a vector of strings.
std::vector<std::string> System::SysFileRead(std::string filename) {
    // Create a vector to hold the individual words read from the system file.
    std::vector<std::string> words;
    
    // Create the path to the system file by concatenating /proc/ with the specified filename.
    std::string path = "/proc/" + filename;

    // Create an input file stream and open the file for reading.
    std::ifstream proc_Sys(path.c_str(), std::ifstream::in);

    // Declare a string variable to store each word read from the file.
    std::string str;

    // Loop through the file, reading each word until there are no more words to read.
    while (proc_Sys >> str) {
        // Add the word to the vector of words using the push_back function.
        words.push_back(str);
    }

    // Close the input file stream as we are done reading the file.
    proc_Sys.close();

    // Return the vector containing all the words read from the system file.
    return words;
}
```

##  Operating System
```cpp
// This function returns the operating system name or version by reading the contents of a specific system file.
std::string System::OperatingSystem() {
    // Create a vector to hold the words read from the system file.
    std::vector<std::string> words = System::SysFileRead("sys/kernel/version");

    // Return the first word in the vector, which represents the operating system name or version.
    return words[0];
}
```

Explanation of the comments:
- The function returns the operating system name or version by reading the contents of a specific system file.
- The function makes use of the previously defined `SysFileRead` function to read the contents of the file located at "/proc/sys/kernel/version". This file typically contains information about the operating system name and version.
- The `words` vector will hold the individual words read from the system file.
- The `SysFileRead` function is called to read the contents of the file. The result is stored in the `words` vector.
- Since the `SysFileRead` function returns a vector of strings and we are interested in the first word, we access the first element of the `words` vector using the index `[0]`.
- Finally, the function returns the first word from the `words` vector, which represents the operating system name or version.

##  Hostname
```cpp
// This function retrieves the hostname of the system by reading the content of the "hostname" system file.
std::string System::Hostname() {
    // Create a vector to store the words read from the "hostname" system file.
    std::vector<std::string> words = System::SysFileRead("sys/kernel/hostname");

    // Return the first word from the vector, which represents the hostname of the system.
    return words[0];
}
```

Explanation of the comments:
- The function retrieves the hostname of the system by reading the content of the "hostname" system file located at `sys/kernel/hostname`.
- The `words` vector is created and initialized by calling the `System::SysFileRead("sys/kernel/hostname")` function. This function reads the content of the "hostname" system file and returns it as a vector of strings, where each word in the file is represented by an element in the vector.
- The `return` statement returns the first word (`words[0]`) from the vector, which represents the hostname of the system. The hostname is the unique name assigned to the system to identify it on the network.

##  Kernel
```cpp
// This function reads the kernel version information from the system file "version" located in the /proc directory and returns the kernel version as a string.
std::string System::Kernel() {
    // Create a vector to hold the individual words read from the "version" system file.
    std::vector<std::string> words = System::SysFileRead("version");
    
    // Return the third word from the vector, which represents the kernel version information.
    // Note: In the file, each word is separated by whitespace, so words[2] will hold the kernel version.
    return words[2];
}
```

Explanation of the comments:
- The function reads the kernel version information from the system file "version" located in the `/proc` directory and returns the kernel version as a string.
- The `words` vector will hold the individual words read from the "version" system file, obtained by calling the `System::SysFileRead` function, which we assume is defined elsewhere.
- The `System::SysFileRead` function reads the contents of the "version" file and stores the words in the `words` vector.
- The function then returns the third word (index 2) from the `words` vector, which represents the kernel version information.
- Note that in the file, each word is separated by whitespace (spaces, tabs, or newlines), so `words[2]` corresponds to the kernel version in the file. Depending on the file format, the specific word that holds the kernel version may vary, so this implementation assumes that the kernel version is at index 2.

##  Total processes
```cpp
// This function returns the total number of processes currently running on the system.

// The function returns an unsigned short, which is a 16-bit integer type, to represent the total number of processes.

unsigned short System::TotalProcesses() {
    // Declare a struct sysinfo variable named 'info'.
    // The 'sysinfo' struct contains various system-related information.
    struct sysinfo info;

    // Call the 'sysinfo' function passing the address of the 'info' struct as an argument.
    // The 'sysinfo' function fills the 'info' struct with system-related information.
    sysinfo(&info);

    // Return the 'procs' member of the 'info' struct, which represents the total number of processes currently running on the system.
    // The 'procs' member is of type unsigned short, matching the return type of the function.
    return info.procs;
}
```

Explanation of the comments:
- The function calculates and returns the total number of processes currently running on the system.
- The function returns an `unsigned short`, which is a 16-bit integer type, suitable for representing the total number of processes.
- Inside the function, a struct `sysinfo` variable named 'info' is declared. The `sysinfo` struct contains various system-related information, including the number of processes.
- The `sysinfo` function is called with the address of the 'info' struct as an argument. This function retrieves system-related information and fills the 'info' struct with the data.
- After calling `sysinfo`, the function returns the `procs` member of the 'info' struct. The `procs` member represents the total number of processes currently running on the system. The `info.procs` value is returned as an `unsigned short`, matching the return type of the function.

##  Processes
```cpp
// This function retrieves information about the running processes on the system and populates a vector of Process objects with the relevant data.
void System::Processes() {
    // Initialize variables to keep track of the current process position and the total number of processes at runtime.
    int process_position = 0;
    int process_number_at_runtime = System::TotalProcesses();
    
    // Declare a pointer to the directory entry structure to be used for reading directory entries.
    struct dirent *dirp;

    // Resize the processes_ vector to accommodate the total number of processes at runtime.
    processes_.resize(process_number_at_runtime);

    // Specify the directory path where the process information is located.
    std::string dir = std::string("/proc");
    
    // Create a vector to store the names of all the entries in the specified directory.
    std::vector<std::string> path_list;
    
    // Open the directory specified by 'dir' and get a pointer to the directory stream.
    DIR *dp;
    dp = opendir(dir.c_str());

    // Read all the entries in the directory and store their names in the 'path_list' vector.
    while ((dirp = readdir(dp)) != NULL) {
        path_list.push_back(std::string(dirp->d_name));
    }

    // Iterate through the 'path_list' vector to process each process.
    for (auto path : path_list) {
        // Check if the 'path' is a valid process ID (an integer greater than 0).
        if (atoi(path.c_str()) > 0) {
            // Convert the 'path' to an integer to get the process ID (pid).
            int pid = atoi(path.c_str());

            // Check if the 'processes_' vector already contains information about this process.
            // If not, create a new Process object, update its data, and add it to the 'processes_' vector.
            if (processes_[process_position].Read_Pid() != pid && process_position < process_number_at_runtime) {
                Process temp_process;
                temp_process.SetPid(pid);
                temp_process.Update();
                processes_[process_position] = temp_process;
            }

            // Move to the next position in the 'processes_' vector.
            process_position++;
        }
    }

    // If the 'process_position' does not match the 'process_number_at_runtime', it means there are fewer processes at runtime than expected.
    // Resize the 'processes_' vector to match the actual number of processes found.
    if (process_position != process_number_at_runtime) {
        processes_.resize(process_position);
    }

    // Close the directory stream.
    closedir(dp);
}
```

Explanation of the function:
- The function retrieves information about the running processes on the system and populates a vector of `Process` objects (`processes_`) with the relevant data.
- It starts by initializing variables to keep track of the current process position (`process_position`) and the total number of processes at runtime (`process_number_at_runtime`).
- The `processes_` vector is then resized to accommodate the total number of processes at runtime.
- It specifies the directory path where the process information is located as `/proc`.
- A vector `path_list` is created to store the names of all the entries in the `/proc` directory.
- The function opens the directory specified by `dir` using `opendir()` and reads all the entries in the directory using `readdir()`. The names of the entries are stored in the `path_list` vector.
- The function then iterates through the `path_list` vector to process each process.
- It checks if the current `path` is a valid process ID (an integer greater than 0).
- If it is a valid process ID, it converts the `path` to an integer to get the process ID (pid).
- It checks if the `processes_` vector already contains information about this process. If not, it creates a new `Process` object, updates its data using the `Update()` function, and adds it to the `processes_` vector at the current `process_position`.
- After processing each process, the `process_position` is incremented to move to the next position in the `processes_` vector.
- If the `process_position` does not match the `process_number_at_runtime`, it means there are fewer processes at runtime than expected. In this case, the `processes_` vector is resized to match the actual number of processes found.
- Finally, the function closes the directory stream using `closedir()`.

##  Memory
These functions are part of a class called `System` and are used to calculate different memory utilization metrics on a Linux system using the `sysinfo` function from the `<sys/sysinfo.h>` header. The `sysinfo` function retrieves system memory information and stores it in the `struct sysinfo` variable `info`.

1. `MemoryUtilization`:
   - This function calculates the memory utilization of the system, which is the percentage of used memory compared to the total available memory.
   - The `sysinfo` function is used to obtain memory information, and the total used memory is calculated as `info.totalram - info.freeram`.
   - The result is returned as a float value representing the memory utilization percentage.

2. `MemoryBuffer`:
   - This function calculates the memory used for buffering data in the system.
   - The `sysinfo` function retrieves memory information, and the total buffered memory is obtained from `info.bufferram`.
   - The result is returned as a float value representing the buffer memory usage percentage.

3. `MemoryShared`:
   - This function calculates the shared memory usage in the system.
   - The `sysinfo` function retrieves memory information, and the total shared memory is obtained from `info.sharedram`.
   - The result is returned as a float value representing the shared memory usage percentage.

4. `MemorySwap`:
   - This function calculates the swap memory utilization, which is the percentage of used swap memory compared to the total available swap memory.
   - The `sysinfo` function retrieves swap memory information, and the total used swap memory is calculated as `info.totalswap - info.freeswap`.
   - The result is returned as a float value representing the swap memory utilization percentage.

Overall, these functions provide a simple way to calculate memory utilization metrics in a Linux system using the `sysinfo` function. They can be used to monitor memory usage and identify potential memory-related issues in the system.

```cpp
#include <sys/sysinfo.h> // Include the header file for sysinfo function

// Function to calculate memory utilization (percentage of used memory)
float System::MemoryUtilization() {
    struct sysinfo info;
    sysinfo(&info); // Retrieve system memory information
    // Calculate memory utilization: (totalram - freeram) / totalram
    return ((float)info.totalram - (float)info.freeram) / (float)info.totalram;
}

// Function to calculate memory used for buffering data
float System::MemoryBuffer() {
    struct sysinfo info;
    sysinfo(&info); // Retrieve system memory information
    // Calculate memory buffer usage: bufferram / totalram
    return (float)info.bufferram / (float)info.totalram;
}

// Function to calculate shared memory usage
float System::MemoryShared() {
    struct sysinfo info;
    sysinfo(&info); // Retrieve system memory information
    // Calculate shared memory usage: sharedram / totalram
    return (float)info.sharedram / (float)info.totalram;
}

// Function to calculate swap memory utilization (percentage of used swap memory)
float System::MemorySwap() {
    struct sysinfo info;
    sysinfo(&info); // Retrieve swap memory information
    // Calculate swap memory utilization: (totalswap - freeswap) / totalswap
    return ((float)info.totalswap - (float)info.freeswap) / (float)info.totalswap;
}
```
Sure, let's explain each calculation formula used in the functions:

1. `MemoryUtilization()`:
   - Formula: `(totalram - freeram) / totalram`
   - Explanation: This formula calculates the memory utilization, which is the percentage of used memory in the system. It subtracts the amount of free memory (`freeram`) from the total available memory (`totalram`) and then divides the result by the total available memory. This gives the proportion of used memory, and by multiplying it by 100, we get the percentage of memory utilization.

2. `MemoryBuffer()`:
   - Formula: `bufferram / totalram`
   - Explanation: This formula calculates the memory used for buffering data. Buffering refers to the temporary storage of data before it is written to a destination or processed further. This metric indicates how much memory is allocated for buffering purposes. It divides the amount of memory used for buffering (`bufferram`) by the total available memory (`totalram`) to get the proportion of memory used for buffering.

3. `MemoryShared()`:
   - Formula: `sharedram / totalram`
   - Explanation: This formula calculates the shared memory usage. Shared memory is memory that can be accessed by multiple processes. This metric indicates how much memory is shared among different processes. It divides the amount of shared memory (`sharedram`) by the total available memory (`totalram`) to get the proportion of memory used for sharing.

4. `MemorySwap()`:
   - Formula: `(totalswap - freeswap) / totalswap`
   - Explanation: This formula calculates the swap memory utilization, which is the percentage of used swap memory. Swap memory is a portion of the hard disk space used as virtual memory when the RAM is fully utilized. It subtracts the amount of free swap memory (`freeswap`) from the total swap memory (`totalswap`) and then divides the result by the total swap memory. This gives the proportion of used swap memory, and by multiplying it by 100, we get the percentage of swap memory utilization.

In summary, each function calculates a specific memory metric and returns the result as a percentage of the total available memory or swap space. These metrics provide valuable information about the system's memory usage and performance.

##  Running Processes
The given function is a member function of a class called `System`, and it is named `RunningProcesses`. Let's break down the function's implementation step by step:

1. `int System::RunningProcesses()`: This line indicates that the function `RunningProcesses` belongs to the class `System` and returns an integer value.

2. `int running_counter = 0;`: This line declares an integer variable named `running_counter` and initializes it to zero. This variable will be used to count the number of running processes.

3. `for (auto &indx_process : processes_) {`: This line starts a loop that iterates through a collection of processes. The loop uses a range-based for loop, where `indx_process` is a reference to each element in the `processes_` collection. The `processes_` collection seems to be a member variable of the `System` class that holds information about different processes.

4. `if (indx_process.Read_Status() == "R") {`: This line checks the status of the current process represented by `indx_process`. It calls the member function `Read_Status()` on the process object to get its status. The condition checks if the status is equal to "R," which typically indicates that the process is running.

5. `running_counter++;`: If the process is found to be running (status is "R"), the `running_counter` variable is incremented by one. This means that the function is counting the number of running processes.

6. `}`: This closing brace indicates the end of the loop.

7. `return running_counter;`: After the loop has finished iterating through all the processes, the function returns the final value of `running_counter`, which represents the total number of running processes in the `processes_` collection.

In summary, the `RunningProcesses` function takes a collection of processes and counts the number of processes that are currently running by checking their status. It returns the count of running processes as an integer.

```cpp
// Function to count the number of running processes in the 'processes_' collection.
// Returns the count of running processes as an integer.
int System::RunningProcesses() {
    // Initialize a counter to keep track of the number of running processes.
    int running_counter = 0;

    // Iterate through the 'processes_' collection using a range-based for loop.
    for (auto &indx_process : processes_) {
        // Check the status of the current process represented by 'indx_process'.
        // The 'Read_Status()' function is called on the process object to get its status.
        // If the status is equal to "R", it means the process is running.
        if (indx_process.Read_Status() == "R") {
            // If the process is running, increment the 'running_counter' by one.
            running_counter++;
        }
    }

    // Return the final count of running processes.
    return running_counter;
}

```