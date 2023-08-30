#pragma once
#define NETWORK_H

#include <vector>
#include <string>
#include <sstream>

// Define a structure to hold interface stats
struct InterfaceStats {
    std::string interfaceName;
    std::string ipAddress;
    std::vector<long long> stats;
};

namespace Network {
std::string GetIPAddress(const std::string &interfaceName);
std::vector<InterfaceStats> ReadNetworkStats();
};  // namespace Network
