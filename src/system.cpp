#include "system.h"

#include <dirent.h>
#include <errno.h>
#include <linux/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "process.h"
#include "processor.h"

std::vector<std::string> System::SysFileRead(std::string filename) {
    std::vector<std::string> words;
    std::string path = "/proc/" + filename;
    std::ifstream proc_Sys(path.c_str(), std::ifstream::in);
    std::string str;
    while (proc_Sys >> str) {
        words.push_back(str);
    }
    proc_Sys.close();
    return words;
}

std::string System::OperatingSystem() {
    std::vector<std::string> words = System::SysFileRead("sys/kernel/version");
    return words[0];
}

std::string System::Hostname() {
    std::vector<std::string> words = System::SysFileRead("sys/kernel/hostname");
    return words[0];
}

std::string System::Kernel() {
    std::vector<std::string> words = System::SysFileRead("version");
    return words[2];
}

Processor &System::Cpu() {
    return System::cpu_;
}

unsigned short System::TotalProcesses() {
    struct sysinfo info;
    sysinfo(&info);
    return info.procs;
}

void System::Processes() {
    std::vector<std::string> path_list;
    DIR *dp;
    dp = opendir("/proc");

    while (struct dirent *dirp = readdir(dp)) {
        path_list.push_back(std::string(dirp->d_name));
    }

    for (const auto &path : path_list) {
        int pid = atoi(path.c_str());
        if (pid <= 0) continue;  // Skip non-process entries

        // Check if the process with the same PID already exists in the vector
        bool found = false;
        for (auto process = processes_.begin(); process != processes_.end();) {
            if (process->Exist()) {
                if (process->Read_Pid() == pid) {
                    found = true;
                }
                ++process;
            } else {
                process = processes_.erase(process);
            }
        }

        if (!found) {
            // If the process with the given PID is not found, add it to the vector
            Process temp_process;
            temp_process.SetPid(pid);
            temp_process.Update();
            processes_.push_back(temp_process);
        }
    }

    closedir(dp);
}

float System::MemoryUtilization() {
    struct sysinfo info;
    sysinfo(&info);
    return ((float)info.totalram - (float)info.freeram) / (float)info.totalram;
}

long System::MemoryTotal() {
    struct sysinfo info;
    sysinfo(&info);
    return info.totalram;
}

long System::MemoryFree() {
    struct sysinfo info;
    sysinfo(&info);
    return info.freeram;
}

float System::MemoryBuffer() {
    struct sysinfo info;
    sysinfo(&info);
    return (float)info.bufferram / (float)info.totalram;
}

float System::MemoryShared() {
    struct sysinfo info;
    sysinfo(&info);
    return (float)info.sharedram / (float)info.totalram;
}

float System::MemorySwap() {
    struct sysinfo info;
    sysinfo(&info);
    return ((float)info.totalswap - (float)info.freeswap) / (float)info.totalswap;
}

long System::MemoryTotalSwap() {
    struct sysinfo info;
    sysinfo(&info);
    return info.totalswap;
}

long System::MemoryFreeSwap() {
    struct sysinfo info;
    sysinfo(&info);
    return info.freeswap;
}

int System::RunningProcesses() {
    int running_counter = 0;
    for (auto indx_process : processes_) {
        running_counter++;
    }
    return running_counter;
}

long int System::UpTime() {
    struct sysinfo info;
    sysinfo(&info);
    return info.uptime;
}

System::System() {
    Processor cpu;
    System::cpu_ = cpu;
}
