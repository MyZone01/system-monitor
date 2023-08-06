#include "process.h"

#include <sys/sysinfo.h>
#include <unistd.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "system.h"

std::vector<std::string> Process::procFileRead(std::string filename) {
    std::vector<std::string> words;
    if (Process::Exist()) {
        std::string str_pid = std::to_string(Process::pid);
        std::string path = "/proc/" + str_pid + "/" + filename;
        std::ifstream proc_pid_status(path.c_str(), std::ifstream::in);
        std::string str;

        while (proc_pid_status >> str) {
            words.push_back(str);
        }
        proc_pid_status.close();
        return words;
    } else {
        std::string NotFoundFlag = "Not Found";
        words.push_back(NotFoundFlag);
        return words;
    }
}

std::string Process::Name() {
    if (Process::Exist()) {
        std::vector<std::string> words = Process::procFileRead("status");
        return words[1];
    } else {
        return "Not Found";
    }
}

std::string Process::Status() {
    if (Process::Exist()) {
        Process::status.clear();
        std::vector<std::string> words = Process::procFileRead("status");
        return words[5] + words[6];
    } else {
        return "Not Found";
    }
}

std::string Process::ParentPid() {
    if (Process::Exist()) {
        std::vector<std::string> words = Process::procFileRead("status");
        return words[14];
    } else {
        return "Not Found";
    }
}

void Process::SetPid(int buff_pid) {
    Process::pid = buff_pid;
}

float Process::CpuUtilization() {
    if (Process::Exist()) {
        float s_time, delta_s_time;
        float u_time, delta_u_time;
        float uptime, delta_uptime;

        std::vector<std::string> stat = procFileRead("stat");
        u_time = (float)(atoi(stat[13].c_str())) / sysconf(_SC_CLK_TCK);
        s_time = (float)(atoi(stat[14].c_str())) / sysconf(_SC_CLK_TCK);
        uptime = (float)Process::UpTime();

        delta_s_time = s_time - Process::prev_s_time;
        delta_u_time = u_time - Process::prev_u_time;
        delta_uptime = uptime - Process::prev_uptime;

        Process::prev_uptime = uptime;
        Process::prev_u_time = u_time;
        Process::prev_s_time = s_time;

        return (delta_s_time + delta_u_time) / delta_uptime;
    } else {
        return 0;
    }
}

std::string Process::Command() {
    if (Process::Exist()) {
        std::vector<std::string> words = Process::procFileRead("comm");
        return words[0];
    } else {
        return "Not Found";
    }
}

std::string Process::Ram() {
    if (Process::Exist()) {
        std::vector<std::string> words = Process::procFileRead("status");

        for (size_t i = 0; i < words.size(); i++) {
            if (words[i] == "VmSize:") {
                return words[++i];
            }
        }
    }
    return "Not Found";
}

std::string Process::User() {
    if (Process::Exist()) {
        std::vector<std::string> words = Process::procFileRead("status");
        return words[19];
    } else {
        return "Not Found";
    }
}

long int Process::UpTime() {
    if (Process::Exist()) {
        long int time = 0;
        std::vector<std::string> stat = procFileRead("stat");
        struct sysinfo info;
        sysinfo(&info);
        time = info.uptime - (atoi(stat[21].c_str()) / sysconf(_SC_CLK_TCK));
        return time;
    } else {
        return 0;
    }
}

bool Process::Exist() {
    std::string str_pid = std::to_string(Process::pid);
    std::string path = "/proc/" + str_pid + "/status";
    ;
    std::ifstream proc_pid_status(path.c_str(), std::ifstream::in);
    bool file_status = (bool)proc_pid_status;
    return file_status;
}

bool& Process::Selected() {
    return Process::selected;
}

void Process::Update() {
    if (Process::Exist()) {
        std::vector<std::string> words = Process::procFileRead("status");
        Process::name = words[1];
        Process::status = words[5] + words[6];
        Process::ppid = words[14];
        Process::uid = words[19];

        for (size_t i = 0; i < words.size(); i++) {
            if (words[i] == "VmSize:")
                Process::ram = words[++i];
        }
        Process::command = Process::Command();
        Process::cpu = Process::CpuUtilization();
        Process::uptime = Process::UpTime();
    }
}

std::string Process::Read_Name() {
    return name;
}
std::string Process::Read_Parent() {
    return Process::ppid;
}
std::string Process::Read_User() {
    return Process::uid;
}
std::string Process::Read_Command() {
    return Process::command;
}
std::string Process::Read_Ram() {
    return Process::ram;
}
std::string Process::Read_Status() {
    return Process::status;
}
int Process::Read_Pid() {
    return Process::pid;
}
float Process::Read_Cpu() {
    return Process::cpu;
}
long int Process::Read_Uptime() {
    return Process::uptime;
}