##  EXIST
The `Exist()` function is a member function of the `Process` class. Its purpose is to check if a process with a specific PID (Process ID) exists on a Linux system. It does this by attempting to open the `/proc/{pid}/status` file, where `{pid}` is the process ID. If the file exists and can be opened successfully, it means the process exists.

Here's a step-by-step explanation of the function:

1. `std::string str_pid = std::to_string(Process::pid);`: This line converts the `pid` member variable of the `Process` class to a string. The `pid` variable holds the process ID that the function wants to check for existence.

2. `std::string path = "/proc/" + str_pid + "/status";`: This line constructs the full path to the `/proc/{pid}/status` file using the previously converted string `str_pid`.

3. `std::ifstream proc_pid_status(path.c_str(), std::ifstream::in);`: This line creates an input file stream (`std::ifstream`) with the `path` as its file name. The file stream will be used to attempt to open the `/proc/{pid}/status` file for reading.

4. `bool file_status = (bool)proc_pid_status;`: This line checks if the file stream `proc_pid_status` is in a valid state. It does this by converting the file stream to a boolean value. In C++, a `std::ifstream` object evaluates to `true` if it is in a valid state and `false` otherwise.

5. `return file_status;`: The function returns the boolean `file_status`, indicating whether the `/proc/{pid}/status` file exists and can be opened. If `file_status` is `true`, it means the process with the specified PID exists; otherwise, it does not.

The `Exist()` function is part of a larger `Process` class, which seems to be a class representing a process on a Linux system. The class provides various member functions to read and access information about the process, such as its name, parent, user, command, CPU utilization, RAM usage, status, and more.

Please note that the function's behavior is specific to Linux systems because it relies on the existence of the `/proc` pseudo-filesystem, which is a special filesystem that exposes information about running processes on Linux. The function may not work on other operating systems that don't have a similar `/proc` filesystem.
##  PROCESS FILE READ
This method, named `procFileRead`, is a member function of the `Process` class. It reads the contents of a specific file associated with a process on a Linux system located in the `/proc/{pid}/` directory, where `{pid}` is the process ID. It returns the contents of the file as a vector of strings, with each element of the vector representing a word from the file.

Here's a step-by-step explanation of the code with comments:

```cpp
std::vector<std::string> Process::procFileRead(std::string filename) {
    // Create an empty vector to store the words read from the file
    std::vector<std::string> words;

    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Convert the process ID (pid) to a string
        std::string str_pid = std::to_string(Process::pid);

        // Create the full path to the file in the /proc/{pid}/ directory
        std::string path = "/proc/" + str_pid + "/" + filename;

        // Open the file for reading using an input file stream (ifstream)
        std::ifstream proc_pid_status(path.c_str(), std::ifstream::in);
        std::string str;

        // Read each word (separated by whitespace) from the file and store it in the vector
        while (proc_pid_status >> str) {
            words.push_back(str);
        }

        // Close the file after reading
        proc_pid_status.close();

        // Return the vector containing the words read from the file
        return words;
    } else {
        // If the process does not exist, return a vector containing a single string "Not Found"
        std::string NotFoundFlag = "Not Found";
        words.push_back(NotFoundFlag);
        return words;
    }
}
```

Here's a summary of what the code does:

1. The function takes a `filename` parameter, which represents the name of the file to read from the `/proc/{pid}/` directory.

2. It creates an empty vector `words` to store the words read from the file.

3. It checks whether the process exists by calling the `Process::Exist()` method. If the process doesn't exist, it returns a vector containing a single element with the string "Not Found."

4. If the process exists, it proceeds to construct the full path to the file using the `filename` and the process ID (`pid`) by converting the `pid` to a string.

5. It opens the file using an input file stream (`ifstream`) and reads each word (sequence of characters separated by whitespace) from the file one by one, pushing each word into the `words` vector.

6. After reading the file, it closes the file stream.

7. It finally returns the vector `words` containing all the words read from the file. If the process does not exist, it will return a vector containing the single string "Not Found."

Overall, this method helps in reading files associated with a specific process in the `/proc` pseudo-filesystem on Linux and handles the case when the process does not exist or the file cannot be read.

##  NAME
This method, named `Name`, is a member function of the `Process` class. Its purpose is to retrieve the name of a specific process on a Linux system. It does this by reading the contents of the `status` file associated with the process in the `/proc/{pid}/` directory (where `{pid}` is the process ID). The `status` file contains various information about the process, including its name.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Process::Name() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Read the contents of the "status" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("status");

        // The name of the process is typically the second word in the "status" file,
        // so return the element at index 1 from the vector.
        return words[1];
    } else {
        // If the process does not exist, return "Not Found"
        return "Not Found";
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to read the contents of the `status` file associated with the process. It does this by calling the `Process::procFileRead("status")` method. This method reads the contents of the `status` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

3. Since the name of the process is typically the second word in the `status` file, the function returns the element at index 1 from the vector `words`, which represents the process name.

4. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns the string "Not Found."

Overall, this method provides a simple and convenient way to retrieve the name of a process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the file cannot be read.

##  STATUS
This method, named `Status`, is a member function of the `Process` class. Its purpose is to retrieve the status of a specific process on a Linux system. It does this by reading the contents of the `status` file associated with the process in the `/proc/{pid}/` directory (where `{pid}` is the process ID). The `status` file contains various information about the process, including its status.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Process::Status() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Clear the current content of the "status" variable in case it has previous data
        Process::status.clear();

        // Read the contents of the "status" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("status");

        // The status of the process is typically represented by the sixth and seventh words in the "status" file,
        // so return the concatenation of the elements at index 5 and 6 from the vector.
        return words[5] + words[6];
    } else {
        // If the process does not exist, return "Not Found"
        return "Not Found";
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to clear the current content of the `status` variable. The purpose of this step is not entirely clear from the provided code snippet, but it seems to ensure that the variable is empty before reading new data from the `status` file.

3. It then reads the contents of the `status` file associated with the process. It does this by calling the `Process::procFileRead("status")` method, which reads the contents of the `status` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

4. The status of the process is typically represented by the sixth and seventh words in the `status` file. The function returns the concatenation of the elements at index 5 and 6 from the vector `words`.

5. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns the string "Not Found."

Overall, this method provides a way to retrieve the status of a process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the file cannot be read. However, the purpose of clearing the `status` variable before reading new data is not entirely clear from the provided snippet, and it may have implications outside the scope of the provided code.

##  PARENT PID
This method, named `ParentPid`, is a member function of the `Process` class. Its purpose is to retrieve the Parent Process ID (PPID) of a specific process on a Linux system. The Parent PID is the ID of the process that spawned or created the current process.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Process::ParentPid() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Read the contents of the "status" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("status");

        // The Parent PID (PPID) of the process is typically represented by the 15th word in the "status" file,
        // so return the element at index 14 from the vector.
        return words[14];
    } else {
        // If the process does not exist, return "Not Found"
        return "Not Found";
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to read the contents of the `status` file associated with the process. It does this by calling the `Process::procFileRead("status")` method, which reads the contents of the `status` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

3. The Parent PID (PPID) of the process is typically represented by the 15th word in the `status` file. The function returns the element at index 14 from the vector `words`, which corresponds to the Parent PID.

4. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns the string "Not Found."

Overall, this method provides a way to retrieve the Parent PID of a process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the file cannot be read. The Parent PID can be useful for understanding the process hierarchy and relationships within the system.

##  CPU UTILIZATION
This method, named `CpuUtilization`, is a member function of the `Process` class. Its purpose is to calculate the CPU utilization of a specific process on a Linux system.

Here's a step-by-step explanation of the code with comments:

```cpp
float Process::CpuUtilization() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Declare variables to store time-related information
        float s_time, delta_s_time;
        float u_time, delta_u_time;
        float uptime, delta_uptime;

        // Read the contents of the "stat" file into a vector of strings
        std::vector<std::string> stat = procFileRead("stat");

        // Extract user mode CPU time and system (kernel) mode CPU time from the "stat" file
        u_time = (float)(atoi(stat[13].c_str())) / sysconf(_SC_CLK_TCK);
        s_time = (float)(atoi(stat[14].c_str())) / sysconf(_SC_CLK_TCK);

        // Get the system uptime (time since boot) from the Process::UpTime() method
        uptime = (float)Process::UpTime();

        // Calculate the differences in CPU times and uptime since the previous calculation
        delta_s_time = s_time - Process::prev_s_time;
        delta_u_time = u_time - Process::prev_u_time;
        delta_uptime = uptime - Process::prev_uptime;

        // Update the previous CPU times and uptime for the next calculation
        Process::prev_uptime = uptime;
        Process::prev_u_time = u_time;
        Process::prev_s_time = s_time;

        // Calculate the CPU utilization as the sum of user mode and system mode CPU times
        // divided by the time elapsed (uptime) since the previous calculation.
        return (delta_s_time + delta_u_time) / delta_uptime;
    } else {
        // If the process does not exist, return 0 (indicating 0% CPU utilization)
        return 0;
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to declare and initialize variables to store time-related information. These variables will be used to calculate the CPU utilization.

3. It reads the contents of the `stat` file associated with the process. It does this by calling the `Process::procFileRead("stat")` method, which reads the contents of the `stat` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

4. It extracts the user mode CPU time (in seconds) and system (kernel) mode CPU time (in seconds) from the `stat` file using the appropriate indices (13 and 14 in the vector `stat`).

5. It retrieves the system uptime (time since boot) using the `Process::UpTime()` method.

6. It calculates the differences (deltas) in system and user CPU times as well as the uptime since the previous calculation. These deltas are used to determine the CPU utilization over a specific time interval.
7. / sysconf(_SC_CLK_TCK): The variable _SC_CLK_TCK is a system configuration parameter that defines the number of clock ticks per second for the CPU. It indicates the granularity of the CPU time measurement. By dividing the user mode CPU time by the number of clock ticks per second, we can obtain the CPU time in seconds. sysconf(_SC_CLK_TCK): The sysconf function is used to retrieve various system configuration values. In this case, it retrieves the number of clock ticks per second.
8. System mode and user mode are two distinct CPU modes that determine how the CPU interacts with the operating system and user applications:

1. **User Mode CPU Time:**
   User mode is a CPU execution mode in which applications and user-level processes run. In this mode, the CPU executes the instructions of the user-level programs, such as applications and processes. These are the tasks that users initiate and interact with. User mode typically includes tasks like running a text editor, web browser, media player, or other applications.

2. **System Mode CPU Time:**
   System mode (also known as kernel mode or privileged mode) is a CPU execution mode in which the operating system kernel and its privileged components run. In this mode, the CPU executes the kernel code, which has access to all hardware resources and controls the system's overall operation. The kernel handles tasks that require higher privileges, such as hardware access, memory management, device drivers, and handling system calls and interrupts.

When a process is running on the CPU, it can transition between user mode and system mode depending on the nature of the task being executed. For example:

- When a user-level application (e.g., a text editor) is running, the CPU is in user mode, executing the application's instructions.
- If the application needs to perform a system-level task, such as writing to a file or accessing hardware, it may request the operating system's services. This request triggers a switch to system mode.
- The operating system kernel then takes control, performs the privileged operation, and returns the result back to the user-level application.
- After completing the system-level task, the CPU returns to user mode, and the application continues executing its instructions.

In summary, user mode CPU time refers to the amount of CPU time spent executing user-level application code, while system mode CPU time refers to the time spent executing privileged kernel code to handle system-level operations. Monitoring the CPU time spent in user mode and system mode can provide valuable insights into how much CPU resources different components of the system are consuming. This information is essential for system administrators and developers to optimize performance and troubleshoot issues.

9. It updates the previous CPU times and uptime with the current values for the next calculation.

10. It calculates the CPU utilization as the sum of the user mode and system mode CPU times divided by the time elapsed (uptime) since the previous calculation. The result represents the CPU utilization as a float value between 0 and 1 (e.g., 0.5 for 50% CPU utilization).

11. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns 0, indicating 0% CPU utilization.

Overall, this method provides a way to calculate the CPU utilization of a specific process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the file cannot be read. The CPU utilization can be useful for monitoring the performance of a specific process.

##  COMMAND
This method, named `Command`, is a member function of the `Process` class. Its purpose is to retrieve the command name (executable name) of a specific process on a Linux system.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Process::Command() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Read the contents of the "comm" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("comm");

        // The command name (executable name) of the process is typically represented by the first word in the "comm" file,
        // so return the element at index 0 from the vector.
        return words[0];
    } else {
        // If the process does not exist, return "Not Found"
        return "Not Found";
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to read the contents of the `comm` file associated with the process. It does this by calling the `Process::procFileRead("comm")` method, which reads the contents of the `comm` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

3. The command name (executable name) of the process is typically represented by the first word in the `comm` file. The function returns the element at index 0 from the vector `words`, which corresponds to the command name.

4. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns the string "Not Found."

Overall, this method provides a way to retrieve the command name (executable name) of a process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the file cannot be read. The command name can be useful for identifying which executable is associated with a specific process.

##  RAM
This method, named `Ram`, is a member function of the `Process` class. Its purpose is to retrieve the RAM usage of a specific process on a Linux system.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Process::Ram() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Read the contents of the "status" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("status");

        // Search for the "VmSize:" entry in the "status" file
        for (size_t i = 0; i < words.size(); i++) {
            if (words[i] == "VmSize:") {
                // If "VmSize:" is found, return the value of the next word (RAM usage)
                return words[++i];
            }
        }
    }

    // If the process does not exist or "VmSize:" entry is not found, return "Not Found"
    return "Not Found";
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to read the contents of the `status` file associated with the process. It does this by calling the `Process::procFileRead("status")` method, which reads the contents of the `status` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

3. The function then searches for the entry "VmSize:" in the `status` file. This entry represents the virtual memory size (RAM usage) of the process.

4. If the "VmSize:" entry is found, the function returns the value of the next word in the vector `words`, which corresponds to the RAM usage of the process.

5. If the process does not exist or the "VmSize:" entry is not found in the `status` file, the function returns the string "Not Found."

Overall, this method provides a way to retrieve the RAM usage of a process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the required information is not available in the `status` file. The RAM usage can be useful for monitoring the memory consumption of a specific process.

##  USER
This method, named `User`, is a member function of the `Process` class. Its purpose is to retrieve the username or user ID associated with a specific process on a Linux system.

Here's a step-by-step explanation of the code with comments:

```cpp
std::string Process::User() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Read the contents of the "status" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("status");

        // The username or user ID of the process is typically represented by the 20th word in the "status" file,
        // so return the element at index 19 from the vector.
        return words[19];
    } else {
        // If the process does not exist, return "Not Found"
        return "Not Found";
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to read the contents of the `status` file associated with the process. It does this by calling the `Process::procFileRead("status")` method, which reads the contents of the `status` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

3. The username or user ID of the process is typically represented by the 20th word in the `status` file. The function returns the element at index 19 from the vector `words`, which corresponds to the username or user ID.

4. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns the string "Not Found."

Overall, this method provides a way to retrieve the username or user ID associated with a process on a Linux system using the `/proc` pseudo-filesystem and handles the case when the process does not exist or the required information is not available in the `status` file. The username or user ID can be useful for identifying which user is running a specific process.

##  UP TIME
This method, named `UpTime`, is a member function of the `Process` class. Its purpose is to calculate the uptime of a specific process on a Linux system. The process uptime is the time that has elapsed since the process was started and has been running.

Here's a step-by-step explanation of the code with comments:

```cpp
long int Process::UpTime() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Declare a variable to store the process uptime
        long int time = 0;

        // Read the contents of the "stat" file into a vector of strings
        std::vector<std::string> stat = procFileRead("stat");

        // Create a struct to store system information, including system uptime
        struct sysinfo info;

        // Get the system information using sysinfo() function
        sysinfo(&info);

        // Calculate the process uptime as the difference between the system uptime and the process start time
        // The process start time is typically represented by the 22nd value in the "stat" file (index 21 in the vector).
        // The value is converted to seconds by dividing by the clock ticks per second (sysconf(_SC_CLK_TCK)).
        time = info.uptime - (atoi(stat[21].c_str()) / sysconf(_SC_CLK_TCK));

        // Return the process uptime
        return time;
    } else {
        // If the process does not exist, return 0 (indicating 0 seconds of uptime)
        return 0;
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to declare a variable `time` to store the process uptime.

3. It reads the contents of the `stat` file associated with the process. It does this by calling the `Process::procFileRead("stat")` method, which reads the contents of the `stat` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

4. It creates a `struct sysinfo` variable named `info` to store system information, including system uptime.

5. It retrieves system information, including system uptime, using the `sysinfo(&info)` function call.

6. The process uptime is calculated as the difference between the system uptime (in seconds) and the process start time. The process start time is typically represented by the 22nd value in the `stat` file (index 21 in the vector `stat`). The value is converted to seconds by dividing it by the clock ticks per second (`sysconf(_SC_CLK_TCK)`).

7. It returns the process uptime in seconds.

8. If the process does not exist (i.e., the `Process::Exist()` method returned `false`), the function returns 0, indicating 0 seconds of uptime.

Overall, this method provides a way to calculate the uptime of a specific process on a Linux system using the `/proc` pseudo-filesystem and the `sysinfo()` function and handles the case when the process does not exist or the file cannot be read. The process uptime is useful for understanding how long a specific process has been running since it started.

##  UPDATE
This method, named `Update`, is a member function of the `Process` class. Its purpose is to update the information associated with a specific process by reading various process attributes from the Linux `/proc` pseudo-filesystem and updating the corresponding member variables of the `Process` class.

Here's a step-by-step explanation of the code with comments:

```cpp
void Process::Update() {
    // Check if the process exists before proceeding
    if (Process::Exist()) {
        // Read the contents of the "status" file into a vector of strings
        std::vector<std::string> words = Process::procFileRead("status");

        // Update the member variables of the Process class with the relevant process attributes

        // Update the process name
        Process::name = words[1];

        // Update the process status by concatenating the 6th and 7th words from the "status" file
        Process::status = words[5] + words[6];

        // Update the Parent PID (PPID) of the process
        Process::ppid = words[14];

        // Update the user ID or username associated with the process
        Process::uid = words[19];

        // Search for the "VmSize:" entry in the "status" file and update the RAM usage
        for (size_t i = 0; i < words.size(); i++) {
            if (words[i] == "VmSize:")
                Process::ram = words[++i];
        }

        // Update the command name (executable name) of the process using the Process::Command() method
        Process::command = Process::Command();

        // Update the CPU utilization of the process using the Process::CpuUtilization() method
        Process::cpu = Process::CpuUtilization();

        // Update the uptime of the process using the Process::UpTime() method
        Process::uptime = Process::UpTime();
    }
}
```

Here's a summary of what the code does:

1. The function first checks whether the process exists by calling the `Process::Exist()` method.

2. If the process exists, it proceeds to read the contents of the `status` file associated with the process. It does this by calling the `Process::procFileRead("status")` method, which reads the contents of the `status` file and returns the data as a vector of strings, with each element of the vector representing a word from the file.

3. The function then updates various member variables of the `Process` class with the corresponding process attributes:

   - The process name is updated with the value at index 1 in the vector `words`.
   - The process status is updated by concatenating the 6th and 7th words from the vector `words`.
   - The Parent PID (PPID) of the process is updated with the value at index 14 in the vector `words`.
   - The user ID or username associated with the process is updated with the value at index 19 in the vector `words`.
   - The RAM usage (virtual memory size) of the process is updated by searching for the "VmSize:" entry in the `status` file and updating it accordingly.
   - The command name (executable name) of the process is updated using the `Process::Command()` method.
   - The CPU utilization of the process is updated using the `Process::CpuUtilization()` method.
   - The uptime of the process is updated using the `Process::UpTime()` method.

By calling this `Update` method, the member variables of the `Process` class are refreshed with the latest information about the process, allowing you to obtain up-to-date data about the process without having to manually retrieve each attribute separately.