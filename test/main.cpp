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

std::vector<std::string> SysFileRead(std::string filename) {
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

int main(int argc, char const *argv[]) {
    std::vector<std::string> words = SysFileRead("version");
    for (auto &&word : words) {
        std::cout << word.c_str();
    }

    return 0;
}
