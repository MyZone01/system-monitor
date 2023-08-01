#include <SDL.h>

#include "header.h"

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

std::vector<std::vector<long long>> ReadNetworkStats() {
    std::ifstream file("/proc/net/dev");
    std::vector<std::vector<long long>> stats;

    if (file) {
        std::string line;
        std::getline(file, line);  // Skip the header line

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string interface;
            std::vector<long long> interfaceStats;

            iss >> interface;
            for (int i = 0; i < 16; i++) {
                long long value;
                iss >> value;
                interfaceStats.push_back(value);
            }

            stats.push_back(interfaceStats);
        }

        file.close();
    }

    return stats;
}

// Function to convert bytes to a human-readable format (GB, MB, KB)
std::string formatBytes(long long bytes) {
    const char *units[] = {"B", "KB", "MB", "GB"};
    int index = 0;

    double value = static_cast<double>(bytes);
    while (value >= 1024.0 && index < 3) {
        value /= 1024.0;
        index++;
    }

    std::stringstream ss;
    ss << std::fixed << value << " " << units[index];

    return ss.str();
}

// Function to calculate the progress value based on received and total bytes
float calculateProgress(long long received, long long total) {
    if (total == 0)
        return 0.0f;

    float progress = static_cast<float>(received) / static_cast<float>(total);
    return std::min(std::max(progress, 0.0f), 1.0f);
}

float GetCPUUsage() {
    std::ifstream file("/proc/stat");

    std::string line;
    std::getline(file, line);

    std::vector<int> values;

    std::stringstream ss(line);

    std::string token;
    ss >> token;

    int value;
    while (ss >> value) {
        values.push_back(value);
    }

    int idle = values[3];
    int total = 0;
    for (int i : values) {
        total += i;
    }

    float usage = 100.0f * (1.0f - (float)idle / (float)total);

    return usage;
}

const char *getOsName() {
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif
}

void systemWindow(const char *id, ImVec2 size, ImVec2 position, char overlay[32], System system, int *fps) {
    const char *OS = getOsName();
    std::string Kernel = system.Kernel();
    int Cores = system.cpu_.CoreCount();
    unsigned short NumberProcess = system.TotalProcesses();
    std::string CPUName = system.cpu_.GetCPUType();
    std::string Hostname = system.Hostname();
    char *username = std::getenv("USER");

    // Variables to control FPS, y-scale, and animation stop
    static float yScale = 100.0f;  // Default y-scale is set to 100
    static bool animation = true;  // Default animation is not stopped
    // static double lastFrameTime = 0.0;  // Variable to store the time of the last frame

    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    ImGui::Text("Operating System used: %s - %s", OS, Kernel.c_str());
    ImGui::Text("Computer Name : %s", Hostname.c_str());
    ImGui::Text("User logged in: %s", username);
    ImGui::Text("CPU Type : %s", CPUName.c_str());
    ImGui::Text("Number of Cores : %d", Cores);
    ImGui::Text("Number of working processes : %d", NumberProcess);

    ImGui::Separator();

    if (ImGui::BeginTabBar("TabBar")) {
        // CPU tab
        if (ImGui::BeginTabItem("CPU")) {
            ImGui::Text("CPU Util: %d [%%]", (int)(system.cpu_usage * 100));
            ImGui::ProgressBar(system.cpu_usage, ImVec2(-1, 0), "");

            ImGui::Text("CPU Average 1 minute: %d [%%]", (int)(system.cpu1m / (float)Cores) * 100);
            ImGui::ProgressBar(system.cpu1m / (float)Cores, ImVec2(-1, 0), "");

            ImGui::Text("CPU Average 5 minute: %d [%%]", (int)(system.cpu5m / (float)Cores) * 100);
            ImGui::ProgressBar(system.cpu5m / (float)Cores, ImVec2(-1, 0), "");

            sprintf(overlay, "CPU Util (45s): \n\nAVG: %d [%%]", (int)(system.cpu1m / (float)Cores * 100));
            ImGui::Text(overlay);

            // Add a checkbox to stop the animation
            ImGui::Checkbox("Animation", &animation);

            // Check if the animation is not stopped before rendering the plot
            if (!animation) {
                *fps = 0;
            } else {
                // Add the first slider bar for controlling FPS
                ImGui::SliderInt("FPS", fps, 0, 60);  // Range from 1 to 60 FPS
            }


            // Add the second slider bar for controlling y-scale
            ImGui::SliderFloat("Y-Scale", &yScale, 5.0f, 100.0f);  // Range from 10 to 1000

            ImGui::PlotLines("", system.cpu_.Cpu_Usage_Log, IM_ARRAYSIZE(system.cpu_.Cpu_Usage_Log), 0, "", 0, yScale, ImVec2(400, 200));
            // ImGui::SameLine();
            // ImGui::PlotLines("", system.cpu_.Cpu_Usage_Log, IM_ARRAYSIZE(system.cpu_.Cpu_Usage_Log), 0, "", 0, 200, ImVec2(400, 200));

            ImGui::EndTabItem();
        }

        // Fan tab
        if (ImGui::BeginTabItem("Fan")) {
            ImGui::EndTabItem();
        }

        // Thermal tab
        if (ImGui::BeginTabItem("Thermal")) {
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void memoryProcessesWindow(const char *id, ImVec2 size, ImVec2 position, System system) {
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Memory Util: %d [%%]", (int)(system.memory_Utilization * 100));
    ImGui::ProgressBar(system.memory_Utilization, ImVec2(-1, 0), "");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Memory Shared: %d [%%]", (int)(system.memory_Shared * 100));
    ImGui::ProgressBar(system.memory_Shared, ImVec2(-1, 0), "");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Memory Buffer: %d [%%]", (int)(system.memory_Buffer * 100));
    ImGui::ProgressBar(system.memory_Buffer, ImVec2(-1, 0), "");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Memory Swap: %d [%%]", (int)(system.memory_Swap * 100));
    ImGui::ProgressBar(system.memory_Swap, ImVec2(-1, 0), "");

    ImGui::Separator();

    int vectorsize = system.processes_.size();
    ImGui::Columns(9, "CPU", true);

    ImGui::Text("PID");
    ImGui::NextColumn();
    ImGui::Text("PPID");
    ImGui::NextColumn();
    ImGui::Text("NAME");
    ImGui::NextColumn();
    ImGui::Text("UID");
    ImGui::NextColumn();
    ImGui::Text("CORE [%%]");
    ImGui::NextColumn();
    ImGui::Text("RAM [KB]");
    ImGui::NextColumn();
    ImGui::Text("UPTIME");
    ImGui::NextColumn();
    ImGui::Text("STATUS");
    ImGui::NextColumn();
    ImGui::Text("COMMAND");
    ImGui::NextColumn();
    for (int i = vectorsize - 1; i >= 0; i--) {
        if (system.processes_[i].Read_Cpu() > 0.01) {
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%d", system.processes_[i].Read_Pid());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", system.processes_[i].Read_Parent().c_str());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", system.processes_[i].Read_Name().c_str());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", system.processes_[i].Read_User().c_str());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%d", (int)(system.processes_[i].Read_Cpu() * 100));
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", system.processes_[i].Read_Ram().c_str());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", Format::ElapsedTime(system.processes_[i].Read_Uptime()).c_str());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", system.processes_[i].Read_Status().c_str());
            ImGui::NextColumn();
            ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "%s", system.processes_[i].Read_Command().c_str());
            ImGui::NextColumn();
        }
    }
    for (int i = vectorsize - 1; i >= 0; i--) {
        if (system.processes_[i].Read_Cpu() < 0.01) {
            ImGui::Text("%d", system.processes_[i].Read_Pid());
            ImGui::NextColumn();
            ImGui::Text("%s", system.processes_[i].Read_Parent().c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", system.processes_[i].Read_Name().c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", system.processes_[i].Read_User().c_str());
            ImGui::NextColumn();
            ImGui::Text("%d", (int)(system.processes_[i].Read_Cpu() * 100));
            ImGui::NextColumn();
            ImGui::Text("%s", system.processes_[i].Read_Ram().c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", Format::ElapsedTime(system.processes_[i].Read_Uptime()).c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", system.processes_[i].Read_Status().c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", system.processes_[i].Read_Command().c_str());
            ImGui::NextColumn();
        }
    }

    ImGui::End();
}

void networkWindow(const char *id, ImVec2 size, ImVec2 position) {
    std::vector<std::vector<long long>> networkStats = ReadNetworkStats();

    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    // Network interfaces table
    ImGui::Text("Network IPv4");
    for (const auto &stats : networkStats) {
        std::string interfaceName = "Interface: " + std::to_string(stats[0]);
        ImGui::Text("%s", interfaceName.c_str());

        ImGui::Columns(9, "network_stats_table");
        ImGui::Text("RX");
        ImGui::NextColumn();
        ImGui::Text("TX");
        ImGui::NextColumn();
        ImGui::Separator();

        std::vector<std::string> rxLabels = {"Bytes", "Packets", "Errs", "Drop", "Fifo", "Frame", "Compressed", "Multicast"};
        std::vector<std::string> txLabels = {"Bytes", "Packets", "Errs", "Drop", "Fifo", "Colls", "Carrier", "Compressed"};

        for (int i = 0; i < 8; i++) {
            ImGui::Text("%s", rxLabels[i].c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", txLabels[i].c_str());
            ImGui::NextColumn();
        }

        ImGui::Separator();

        for (int i = 0; i < 8; i++) {
            std::string rxValue = formatBytes(stats[i + 1]);
            std::string txValue = formatBytes(stats[i + 9]);

            ImGui::Text("%s", rxValue.c_str());
            ImGui::NextColumn();
            ImGui::Text("%s", txValue.c_str());
            ImGui::NextColumn();
        }

        ImGui::Separator();
    }

    ImGui::Columns(1);

    // RX section
    ImGui::Separator();
    ImGui::Text("RX Section");

    for (const auto &stats : networkStats) {
        std::string interfaceName = "Interface: " + std::to_string(stats[0]);
        ImGui::Text("%s", interfaceName.c_str());

        long long receivedBytes = stats[1];
        long long totalBytes = receivedBytes + stats[8];

        float rxProgress = calculateProgress(receivedBytes, totalBytes);
        std::string rxProgressLabel = "RX Progress##" + std::to_string(stats[0]);
        ImGui::ProgressBar(rxProgress, ImVec2(-1, 0), rxProgressLabel.c_str());

        std::string receivedBytesLabel = "Received: " + formatBytes(receivedBytes);
        std::string totalBytesLabel = "Total: " + formatBytes(totalBytes);
        ImGui::Text("%s", receivedBytesLabel.c_str());
        ImGui::Text("%s", totalBytesLabel.c_str());

        ImGui::Separator();
    }

    // TX section
    ImGui::Separator();
    ImGui::Text("TX Section");

    for (const auto &stats : networkStats) {
        std::string interfaceName = "Interface: " + std::to_string(stats[0]);
        ImGui::Text("%s", interfaceName.c_str());

        long long transmittedBytes = stats[9];
        long long totalBytes = transmittedBytes + stats[16];

        float txProgress = calculateProgress(transmittedBytes, totalBytes);
        std::string txProgressLabel = "TX Progress##" + std::to_string(stats[0]);
        ImGui::ProgressBar(txProgress, ImVec2(-1, 0), txProgressLabel.c_str());

        std::string transmittedBytesLabel = "Transmitted: " + formatBytes(transmittedBytes);
        std::string totalBytesLabel = "Total: " + formatBytes(totalBytes);
        ImGui::Text("%s", transmittedBytesLabel.c_str());
        ImGui::Text("%s", totalBytesLabel.c_str());

        ImGui::Separator();
    }

    ImGui::End();
}

int main(int, char **) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("System Monitor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char *name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
    bool err = false;
#endif
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    char overlay[32];
    System system;
    static int fps = 30;  // Default FPS is set to 30

    std::thread Updater1(Updater::ProcessesUpdater, &system);
    Updater1.detach();
    std::thread Updater(Updater::CPUUpdater, &system, &fps);
    Updater.detach();

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        {
            ImVec2 mainDisplay = io.DisplaySize;
            memoryProcessesWindow("== Memory and Processes ==",
                                  ImVec2((mainDisplay.x / 2) - 20, (mainDisplay.y / 2) + 30),
                                  ImVec2((mainDisplay.x / 2) + 10, 10), system);

            systemWindow("== System ==",
                         ImVec2((mainDisplay.x / 2) - 10, (mainDisplay.y / 2) + 30),
                         ImVec2(10, 10), overlay, system, &fps);

            networkWindow("== Network ==",
                          ImVec2(mainDisplay.x - 20, (mainDisplay.y / 2) - 60),
                          ImVec2(10, (mainDisplay.y / 2) + 50));
        }

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
