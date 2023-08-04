#include "processor.h"

#include <sys/sysinfo.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <numeric>
#include <vector>

std::vector<size_t> get_cpu_times() {
    std::ifstream proc_stat("/proc/stat");

    proc_stat.ignore(5, ' ');
    std::vector<size_t> times;
    for (size_t time; proc_stat >> time; times.push_back(time));
    return times;
}

bool get_cpu_times(size_t &idle_time, size_t &total_time) {
    const std::vector<size_t> cpu_times = get_cpu_times();
    if (cpu_times.size() < 4)
        return false;
    idle_time = cpu_times[3];
    total_time = std::accumulate(cpu_times.begin(), cpu_times.end(), 0);
    return true;
}

float Processor::Utilization() {
    size_t idle_time = 0, total_time = 0;
    get_cpu_times(idle_time, total_time);
    const float idle_time_delta = idle_time - Processor::CPU_Previous_Idle;
    const float total_time_delta = total_time - Processor::CPU_Previous_Total;
    const float utilization = (1.0 - idle_time_delta / total_time_delta);
    Processor::CPU_Previous_Idle = idle_time;
    Processor::CPU_Previous_Total = total_time;

    for (int j = 0; j < 29; j++) {
        Processor::Cpu_Usage_Log[j] = Processor::Cpu_Usage_Log[j + 1];
    }
    Processor::Cpu_Usage_Log[29] = utilization * 100;
    return utilization;
}

int Processor::CoreCount() {
    std::string Cpucount;
    std::vector<std::string> words;
    std::string path = "/proc/cpuinfo";
    std::ifstream proc_Sys(path.c_str(), std::ifstream::in);
    std::string str;
    while (proc_Sys >> str) {
        words.push_back(str);
    }
    proc_Sys.close();
    for (size_t i = 0; i < words.size(); i++) {
        if (words[i] == "siblings") {
            Cpucount = words[i + 2];
        }
    }
    Processor::Cpu_Count = atoi(Cpucount.c_str());
    return Processor::Cpu_Count;
}

std::string Processor::GetCPUType() {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        return "";
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0) {
            char* colon = strchr(line, ':');
            if (colon) {
                return colon;
            }
            break;
        }
    }
    fclose(fp);
    return "";
}

float Processor::CpuMean1m() {
    struct sysinfo info;
    sysinfo(&info);

    float avgload = info.loads[0] / 100000.0;
    return avgload;
}

float Processor::CpuMean5m() {
    struct sysinfo info;
    sysinfo(&info);

    float avgload = info.loads[1] / 100000.0;
    return avgload;
}

Processor::Processor(void) {
    for (auto &usage : Cpu_Usage_Log) {
        usage = 0;
    }
}