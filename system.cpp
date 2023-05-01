#include "header.h"

// get cpu id and information, you can use `proc/cpuinfo`
string CPUinfo()
{
    char CPUBrandString[0x40];
    unsigned int CPUInfo[4] = {0, 0, 0, 0};

    // unix system
    // for windoes maybe we must add the following
    // __cpuid(regs, 0);
    // regs is the array of 4 positions
    __cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
    unsigned int nExIds = CPUInfo[0];

    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    for (unsigned int i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);

        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    string str(CPUBrandString);
    return str;
}

// getOsName, this will get the OS of the current computer
const char *getOsName()
{
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif
}

const char* getUser() {
#ifdef _WIN32
    DWORD usernameSizeWin = usernameSize;
    return GetUserName(username, &usernameSizeWin);
#else
    struct passwd* pw = getpwuid(getuid());
    std::string username = pw->pw_name;
    return username.c_str();
#endif
}

const char* getComputerName() {
    char name[11];
    size_t size = sizeof(name);
    if (gethostname(name, size) == 0) {
        return std::string(name, size).c_str();
    } else {
        return "";
    }
}

const char* getCPUType() {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if(fp == NULL) {
        return "";
    }
    char line[256];
    while(fgets(line, sizeof(line), fp)) {
        if(strncmp(line, "model name", 10) == 0) {
            char* colon = strchr(line, ':');
            if(colon) {
                return colon;
            }
            break;
        }
    }
    fclose(fp);
    return "";
}

// Utility function to run a command and return its output as a string
std::string runCommand(const char* cmd) {
    std::vector<char> buffer(128);
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    auto returnCode = pclose(pipe);
    if (returnCode != 0) throw std::runtime_error("command failed");
    return result;
}

// main function to display system information
void DrawTaskManager() {
     // Get process information using ps command
    std::string psOutput = runCommand("ps -e -o state=");
    int running = 0, sleeping = 0, uninterruptible = 0, zombie = 0, tracedStopped = 0, interrupted = 0, total = 0;
    for (char& c : psOutput) {
        switch (c) {
            case 'R': total++; running++; break;
            case 'S': total++; sleeping++; break;
            case 'D': total++; uninterruptible++; break;
            case 'Z': total++; zombie++; break;
            case 'T': total++; tracedStopped++; break;
            case 't': total++; interrupted++; break;
        }
    }

    // Display process information using Dear ImGUI
    ImGui::Text("Total number of processes: %d", total);
    ImGui::Text("Running: %d", running);
    ImGui::Text("Sleeping: %d", sleeping);
    ImGui::Text("Uninterruptible: %d", uninterruptible);
    ImGui::Text("Zombie: %d", zombie);
    ImGui::Text("Traced/stopped: %d", tracedStopped);
    ImGui::Text("Interrupted: %d", interrupted);
}