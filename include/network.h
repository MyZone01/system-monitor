#include "imgui.h"
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Function to draw the Network section UI
void DrawNetworkSection()
{
    // Fetch Network information using ifaddrs
    struct ifaddrs* ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == 0)
    {
        ImGui::Text("Network Interfaces:");

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL)
                continue;

            // Check if it's an IPv4 interface
            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                struct sockaddr_in* sa = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
                char ipAddr[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(sa->sin_addr), ipAddr, INET_ADDRSTRLEN);

                // Draw the network interface and its IP address
                ImGui::Text("%s: %s", ifa->ifa_name, ipAddr);
            }
        }

        freeifaddrs(ifaddr);
    }
}
