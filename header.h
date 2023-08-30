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
#include "include/network.h"
#include "include/process.h"
#include "include/processor.h"
#include "include/system.h"
#include "include/updater.h"
#include "include/fan.h"

using namespace std;

string CPUinfo();
const char* getOsName();
const char* getUser();
const char* getComputerName();
const char* getCPUType();
void DrawTaskManager();

#endif
