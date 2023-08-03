#include "imgui.h"
#include <sys/statvfs.h>

// Function to draw the Disk Usage UI
void DrawDiskUsage()
{
    // Fetch Disk Usage information using statvfs
    struct statvfs stat;
    if (statvfs("/", &stat) == 0)
    {
        unsigned long long totalSpace = stat.f_frsize * stat.f_blocks;
        unsigned long long freeSpace = stat.f_frsize * stat.f_bfree;
        float diskUsage = static_cast<float>(totalSpace - freeSpace) / totalSpace * 100.0f;

        // Draw Disk Usage UI
        ImGui::Text("Disk Usage: %.1f%%", diskUsage);
        ImGui::ProgressBar(diskUsage / 100.0f, ImVec2(-1, 0), ""); // Convert to fraction (0 to 1)
    }
}
