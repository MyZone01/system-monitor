#include "updater.h"

#include <chrono>
#include <iostream>
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

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Updater::CPUUpdater(System* system, int* targetFPS) {
    while (true) {
        int updateInterval = 1000;
        if (*targetFPS > 0) {
            updateInterval = 1000 / *targetFPS;

            system->cpu_usage = system->cpu_.Utilization();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
}

void Updater::FanSpeedUpdater(Fan* fan, int* targetFPS) {
    while (true) {
        int updateInterval = 1000;
        if (*targetFPS > 0) {
            updateInterval = 1000 / *targetFPS;
            fan->GetFanSpeedOnLinux();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
}

void Updater::TemperatureUpdater(Fan* fan, int* targetFPS) {
    while (true) {
        int updateInterval = 1000;
        if (*targetFPS > 0) {
            updateInterval = 1000 / *targetFPS;
            fan->GetCPUTemperatureOnLinux();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
}
