#include "fan.h"

#include <fstream>
#include <string>

std::string Fan::GetFanStatusOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_enable" with the correct path to the fan status file
    std::ifstream statusFile("/sys/class/hwmon/hwmon1/temp1_input");
    std::string status;
    statusFile >> status;
    auto _status = atoi(status.c_str());
    return _status >= 1 ? "Enabled" : "Disabled";
}

float Fan::GetFanSpeedOnLinux() {
    // Replace "/sys/class/hwmon/hwmon1/fanX_input" with the correct path to the fan speed file
    std::ifstream speedFile("/sys/class/hwmon/hwmon1/temp1_input");
    int _speed;
    speedFile >> _speed;
    float speed = static_cast<float>(_speed);

    for (int i = 0; i < 29; i++) {
        Fan::fan_speed[i] = Fan::fan_speed[i + 1];
    }
    Fan::fan_speed[29] = speed * 100;
    return speed;
}

float Fan::GetCPUTemperatureOnLinux() {
    // Replace "/sys/class/thermal/thermal_zoneX/temp" with the correct path to the CPU temperature file
    std::ifstream tempFile("/sys/class/thermal/thermal_zone6/temp");
    int temp;
    tempFile >> temp;
    return static_cast<float>(temp);  // Temperature in degrees Celsius
}

std::string Fan::GetBatteryLevel() {
    std::string batteryPath = "/sys/class/power_supply/BAT1/capacity";  // Update the path based on your system
    std::ifstream file(batteryPath);
    std::string level;
    file >> level;
    return level.c_str();
}

Fan::Fan() {
}