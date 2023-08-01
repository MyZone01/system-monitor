#include "updater.h"

#include <chrono>
#include <thread>

#include "system.h"
void Updater::ProcessesUpdater(System* system) {
    while (true) {
        system->Processes();
        for (auto& proc : system->processes_) {
            proc.Update();
        }
        system->total_processes = system->TotalProcesses();
        system->memory_Utilization = system->MemoryUtilization();
        system->memory_Shared = system->MemoryShared();
        system->memory_Swap = system->MemorySwap();
        system->memory_Buffer = system->MemoryBuffer();
        system->cpu1m = system->cpu_.CpuMean1m();
        system->cpu5m = system->cpu_.CpuMean5m();

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
void Updater::CPUUpdater(System* system, int* targetFPS) {
    // Keep track of the last update time using std::chrono::steady_clock
    // std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();

    while (true) {
        // Calculate the update interval based on targetFPS
        int updateInterval = 1000;
        if (*targetFPS > 0) {
            updateInterval = 1000 / *targetFPS;
            // Calculate the time difference since the last update
            // std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            // std::chrono::milliseconds timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdate);

            // // If the time difference is greater than or equal to the update interval, update the CPU utilization and reset the last update time
            // if (timeDifference.count() >= updateInterval) {
            system->cpu_usage = system->cpu_.Utilization();
            // }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
}

// void Updater::ProcessesUpdater(System* system, int* targetFPS) {
//     // Keep track of the last update time using std::chrono::steady_clock
//     std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();

//     while (true) {
//         system->Processes();
//         for (auto& proc : system->processes_) {
//             proc.Update();
//         }
//         // Calculate the update interval based on targetFPS
//         const int updateInterval = 1000 / *targetFPS;
//         // Calculate the time difference since the last update
//         std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
//         std::chrono::milliseconds timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdate);

//         // If the time difference is greater than or equal to the update interval, update the CPU utilization and reset the last update time
//         if (timeDifference.count() >= updateInterval) {
//             system->total_processes = system->TotalProcesses();
//             system->memory_Utilization = system->MemoryUtilization();
//             system->memory_Shared = system->MemoryShared();
//             system->memory_Swap = system->MemorySwap();
//             system->memory_Buffer = system->MemoryBuffer();
//             system->cpu1m = system->cpu_.CpuMean1m();
//             system->cpu5m = system->cpu_.CpuMean5m();
//             system->cpu_usage = system->cpu_.Utilization();
//             lastUpdate = currentTime;  // Reset the last update time to the current time
//         }

//         std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//     }
// }
