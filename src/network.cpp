#include "network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <net/if.h>
#include <string>

using namespace std;

string Network::GetIPAddress(const string &interfaceName) {
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        return "";
    }

    close(fd);

    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

vector<InterfaceStats> Network::ReadNetworkStats() {
    ifstream file("/proc/net/dev");
    vector<InterfaceStats> stats;

    if (file) {
        string line;
        getline(file, line);  // Skip the header line
        getline(file, line);  // Skip the header line

        while (getline(file, line)) {
            istringstream iss(line);
            string interface;
            vector<long long> interfaceStats;

            iss >> interface;
            interface = interface.substr(0, interface.find_last_of(':'));
            for (int i = 0; i < 16; i++) {
                long long value;
                iss >> value;
                interfaceStats.push_back(value);
            }

            string ipAddress = Network::GetIPAddress(interface);

            // Create an InterfaceStats object and add it to the vector
            stats.push_back({interface, ipAddress, interfaceStats});
        }

        file.close();
    }

    return stats;
}

