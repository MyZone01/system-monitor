#pragma once
#define PROCESSOR_H

#include <string>

class Processor {
   public:
    Processor();

    std::string GetCPUType();
    float Utilization();
    int CoreCount();
    float CpuMean1m();
    float CpuMean5m();
    float Cpu_Usage_Log[30];

   private:
    int Cpu_Count;
    float CPU_Previous_Idle = 0;
    float CPU_Previous_Total = 0;
};
