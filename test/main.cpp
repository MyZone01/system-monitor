#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function to fetch disk usage information for /dev/sdc on WSL
std::pair<long long, long long> GetDiskUsageOnWSL() {
    // Open a pipe to run the 'df' command and read its output
    FILE* pipe = popen("df -B1 /dev/sdc", "r");
    if (!pipe) {
        std::cerr << "Error opening pipe to df command." << std::endl;
        return std::make_pair(-1, -1);
    }

    // Buffer to read the command output
    char buffer[1024];

    // Read the output line by line and parse the disk usage information
    std::string diskInfo;
    fgets(buffer, sizeof(buffer), pipe);
    fgets(buffer, sizeof(buffer), pipe);
    diskInfo = buffer;

    // Close the pipe
    pclose(pipe);

    // Parse the disk usage information
    std::istringstream iss(diskInfo);
    std::string filesystem, size, used, avail, usePercent, mountedOn;
    iss >> filesystem >> size >> used >> avail >> usePercent >> mountedOn;

    // Convert the size and used values from strings to long long (in bytes)
    long long sizeBytes, usedBytes;
    sizeBytes = std::stoll(size);
    usedBytes = std::stoll(used);

    return std::make_pair(sizeBytes, usedBytes);
}

// Example usage
int main() {
    std::pair<long long, long long> diskUsageInfo = GetDiskUsageOnWSL();

    if (diskUsageInfo.first != -1 && diskUsageInfo.second != -1) {
        // Print the size and used part of /dev/sdc in bytes
        std::cout << "Size of /dev/sdc: " << diskUsageInfo.first << " bytes" << std::endl;
        std::cout << "Used part of /dev/sdc: " << diskUsageInfo.second << " bytes" << std::endl;
    } else {
        std::cout << "Error fetching disk usage information for /dev/sdc." << std::endl;
    }

    return 0;
}
