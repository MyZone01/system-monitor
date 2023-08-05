#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string formatBytes(long bytes) {
    const char *units[] = {"B", "KB", "MB", "GB"};
    int index = 0;

    double value = static_cast<double>(bytes);
    while (value >= 1024.0 && index < 3) {
        value /= 1024.0;
        index++;
    }

    std::stringstream ss;
    ss << std::fixed << value << " " << units[index];

    return ss.str();
}

int main() {
    std::cout << formatBytes((long)755738) << std::endl;
    std::cout << formatBytes((long)452755738) << std::endl;
    std::cout << formatBytes((long)1452755738) << std::endl;

    return 0;
}

// 452755738 bytes => 431.78 MB. // perfect
// 452755738 bytes => 0.42 GB. // too small
// 452755738 bytes => 442144.28.6 KB. // too big