#include <iostream>
#include <sys/statvfs.h>

int main() {
    struct statvfs buffer;

    // Replace "/" with the root path
    const char* root_path = "/";

    if (statvfs(root_path, &buffer) == 0) {
        // Disk Size, Free Space, and Used Space in bytes
        unsigned long long total_space = buffer.f_frsize * buffer.f_blocks;
        unsigned long long free_space = buffer.f_frsize * buffer.f_bfree;
        unsigned long long used_space = total_space - free_space;

        // Calculate Disk Usage Percentage
        double disk_usage_percentage = 100.0 * used_space / total_space;

        std::cout << "Root Path: " << root_path << std::endl;
        std::cout << "Total Disk Space: " << total_space << " bytes" << std::endl;
        std::cout << "Used Disk Space: " << used_space << " bytes" << std::endl;
        std::cout << "Free Disk Space: " << free_space << " bytes" << std::endl;
        std::cout << "Disk Usage: " << disk_usage_percentage << "%" << std::endl;
    } else {
        std::cerr << "Error getting disk information for path: " << root_path << std::endl;
    }

    return 0;
}
