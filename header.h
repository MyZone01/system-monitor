#ifndef header_H
#define header_H

#include <arpa/inet.h>
#include <cpuid.h>
#include <dirent.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <limits.h>
#include <netinet/in.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <cstdio>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <array>
#include <vector>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "include/format.h"
#include "include/mem.h"
#include "include/network.h"
#include "include/process.h"
#include "include/processor.h"
#include "include/system.h"
#include "include/updater.h"
#include "include/fan.h"

using namespace std;

struct CPUStats {
    long long int user;
    long long int nice;
    long long int system;
    long long int idle;
    long long int iowait;
    long long int irq;
    long long int softirq;
    long long int steal;
    long long int guest;
    long long int guestNice;
};

struct Proc {
    int pid;
    string name;
    char state;
    long long int vsize;
    long long int rss;
    long long int utime;
    long long int stime;
};

struct IP4 {
    char* name;
    char addressBuffer[INET_ADDRSTRLEN];
};

struct Networks {
    vector<IP4> ip4s;
};

struct TX {
    int bytes;
    int packets;
    int errs;
    int drop;
    int fifo;
    int frame;
    int compressed;
    int multicast;
};

struct RX {
    int bytes;
    int packets;
    int errs;
    int drop;
    int fifo;
    int colls;
    int carrier;
    int compressed;
};

string CPUinfo();
const char* getOsName();
const char* getUser();
const char* getComputerName();
const char* getCPUType();
void DrawTaskManager();

#endif
