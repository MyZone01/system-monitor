#include "format.h"
#include <sstream>
#include <time.h>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long int seconds) {
    time_t rawTime = seconds;
    struct tm *ptm = gmtime(&rawTime);

    std::string H = std::to_string(ptm->tm_hour).c_str();
    std::string M = std::to_string(ptm->tm_min).c_str();
    std::string S = std::to_string(ptm->tm_sec).c_str();

    std::string time = H + ":" + M + ":" + S;

    return time;
}

// Function to convert bytes to a human-readable format (GB, MB, KB)
std::string Format::ReadableSize(long bytes) {
    const char *units[] = {"B", "KB", "MB", "GB"};
    int index = 0;

    double value = static_cast<double>(bytes);
    while (value >= 1024.0 && index < 3) {
        value /= 1024.0;
        index++;
    }

    std::stringstream ss;

    if (index == 0) {  // No need for decimals for Bytes
        ss << static_cast<long>(value) << " " << units[index];
    } else {
        long wholePart = static_cast<long>(value);
        long decimalPart = static_cast<long>((value - wholePart) * 100);  // Get two decimal places
        ss << wholePart << ".";

        if (decimalPart < 10) {
            ss << "0";  // To ensure two decimal digits
        }

        ss << decimalPart << " " << units[index];
    }

    return ss.str();
}

