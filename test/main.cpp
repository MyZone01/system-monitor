#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream fanFile("/proc/acpi/ibm/fan");
    if (!fanFile.is_open()) {
        std::cerr << "Failed to open /proc/acpi/ibm/fan" << std::endl;
        return 1;
    }

    std::string _speed;
    fanFile >> _speed;
    fanFile >> _speed;
    fanFile >> _speed;
    fanFile >> _speed;
    // fanFile >> _speed;

    int speed = atoi(_speed.c_str());

    std::cout << speed << std::endl;

    // std::string line;
    // while (std::getline(fanFile, line)) {
    //     if (line.find("status:") != std::string::npos) {
    //         std::cout << "Status: " << line.substr(9) << std::endl;
    //     } else if (line.find("level:") != std::string::npos) {
    //         std::cout << "Level: " << line.substr(8) << std::endl;
    //     } else if (line.find("speed:") != std::string::npos) {
    //         std::cout << "Speed: " << line.substr(8) << std::endl;
    //     }
    // }

    fanFile.close();
    return 0;
}

// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>

// int main() {
//     FILE* file = popen("df --output=size,used,avail /", "r");
//     if (!file) {
//         std::cerr << "Failed to run df command" << std::endl;
//         return 1;
//     }

//     // We assume each line is less than 256 characters.
//     char line[256];

//     // Read header
//     if (!fgets(line, sizeof(line), file)) {
//         std::cerr << "Failed to read df output" << std::endl;
//         return 1;
//     }

//     // Read values
//     if (!fgets(line, sizeof(line), file)) {
//         std::cerr << "Failed to read df output" << std::endl;
//         return 1;
//     }

//     unsigned long long totalSpace, usedSpace, availSpace;
//     sscanf(line, "%llu %llu %llu", &totalSpace, &usedSpace, &availSpace);

//     // Calculate percentages
//     double usedPercent = static_cast<double>(usedSpace) / totalSpace * 100.0;
//     double availPercent = static_cast<double>(availSpace) / totalSpace * 100.0;

//     // Print results
//     std::cout << "Total space: " << totalSpace * 1024 << " Bytes" << std::endl; // assuming df returns values in 1K-blocks
//     std::cout << "Used space: " << usedSpace * 1024 << " Bytes (" << usedPercent << "%)" << std::endl;
//     std::cout << "Available space: " << availSpace * 1024 << " Bytes (" << availPercent << "%)" << std::endl;

//     pclose(file);
//     return 0;
// }

// #include <iostream>
// #include <sys/types.h>
// #include <sys/statvfs.h>
// #include <string>
// #include <sstream>
// #include <iomanip>

// std::string formatBytes(long bytes) {
//     const char *units[] = {"B", "KB", "MB", "GB"};
//     int index = 0;

//     double value = static_cast<double>(bytes);
//     while (value >= 1024.0 && index < 3) {
//         value /= 1024.0;
//         index++;
//     }

//     std::stringstream ss;
//     ss << std::fixed << value << " " << units[index];

//     return ss.str();
// }

// int main() {
//     struct statvfs buf;

//     if (statvfs("/", &buf) != 0) {
//         std::cerr << "statvfs failed" << std::endl;
//         return 1;
//     }

    // unsigned long long totalSpace = buf.f_blocks * buf.f_frsize;
    // unsigned long long freeSpace = buf.f_bfree * buf.f_frsize;
    // unsigned long long availSpace = buf.f_bavail * buf.f_frsize;

//     std::cout << "Total space: " << formatBytes(totalSpace).c_str() << " Bytes" << std::endl;
//     std::cout << "Free space: " << formatBytes(freeSpace).c_str() << " Bytes" << std::endl;
//     std::cout << "Available space: " << formatBytes(availSpace).c_str() << " Bytes" << std::endl;

//     return 0;
// }


// // int main() {
// //     std::cout << formatBytes((long)755738) << std::endl;
// //     std::cout << formatBytes((long)452755738) << std::endl;
// //     std::cout << formatBytes((long)1452755738) << std::endl;

// //     return 0;
// // }

// // // 452755738 bytes => 431.78 MB. // perfect
// // // 452755738 bytes => 0.42 GB. // too small
// // // 452755738 bytes => 442144.28.6 KB. // too big