#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main(int argc, char **argv) {
    FILE *fp;
    char ps_command[1024];
    char ps_output[4096];

    sprintf(ps_command, "ps -e -o state");

    fp = popen(ps_command, "r");
    if (fp == NULL) {
        cout << "Error: failed to execute the ps command" << endl;
        return 1;
    }

    int running = 0;
    int sleeping = 0;
    int uninterruptible = 0;
    int zombie = 0;
    int traced_stopped = 0;
    int interrupted = 0;

    while (fgets(ps_output, sizeof(ps_output), fp) != NULL) {
        if (strncmp(ps_output, "R", 1) == 0) {
            running++;
        } else if (strncmp(ps_output, "S", 1) == 0) {
            sleeping++;
        } else if (strncmp(ps_output, "D", 1) == 0) {
            uninterruptible++;
        } else if (strncmp(ps_output, "Z", 1) == 0) {
            zombie++;
        } else if (strncmp(ps_output, "T", 1) == 0) {
            traced_stopped++;
        } else if (strncmp(ps_output, "t", 1) == 0) {
            interrupted++;
        }
    }

    pclose(fp);

    cout << "Running: " << running << endl;
    cout << "Sleeping: " << sleeping << endl;
    cout << "Uninterruptible: " << uninterruptible << endl;
    cout << "Zombie: " << zombie << endl;
    cout << "Traced/Stopped: " << traced_stopped << endl;
    cout << "Interrupted: " << interrupted << endl;

    return 0;
}
