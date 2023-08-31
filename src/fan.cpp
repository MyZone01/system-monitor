#include "fan.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

bool pathExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string Fan::GetFanStatusOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_enable" with the correct path to the fan status file
    std::ifstream fanFile("/proc/acpi/ibm/fan");
    std::string status;
    fanFile >> status;
    fanFile >> status;
    return status;
}

std::string Fan::GetFanLevelOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_enable" with the correct path to the fan status file
    std::ifstream fanFile("/proc/acpi/ibm/fan");
    std::string level;
    fanFile >> level;
    fanFile >> level;
    fanFile >> level;
    fanFile >> level;
    fanFile >> level;
    fanFile >> level;
    return level;
}

float Fan::GetFanSpeedOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_input" with the correct path to the fan speed file
    std::ifstream fanFile("/proc/acpi/ibm/fan");
    std::string _speed;
    fanFile >> _speed;
    fanFile >> _speed;
    fanFile >> _speed;
    fanFile >> _speed;
    float speed = static_cast<float>(atoi(_speed.c_str()));

    for (int i = 0; i < 99; i++) {
        Fan::fan_speed_log[i] = Fan::fan_speed_log[i + 1];
    }
    Fan::fan_speed_log[99] = speed / 1000.0f;
    // std::cout << fan_speed_log[99] << std::endl;
    return speed;
}

float Fan::GetCPUTemperatureOnLinux() {
    // Replace "/sys/class/thermal/thermal_zoneX/temp" with the correct path to the CPU temperature file
    std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
    int _temp;
    tempFile >> _temp;
    float temp = static_cast<float>(_temp);  // Temperature in degrees Celsius
    for (int i = 0; i < 99; i++) {
        Fan::temp_log[i] = Fan::temp_log[i + 1];
    }
    Fan::temp_log[99] = temp / 1000.0f;
    return temp;
}

std::string Fan::GetBatteryLevel() {
    std::string batteryPath = "/sys/class/power_supply/BAT0/capacity";  // Update the path based on your system
    if (!pathExists(batteryPath)) {
        batteryPath = "/sys/class/power_supply/BAT1/capacity";  // Update the path based on your system
    } 
    std::ifstream file(batteryPath);
    std::string level;
    file >> level;
    return level.c_str();
}
