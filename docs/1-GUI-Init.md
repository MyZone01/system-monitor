1. ImGUI (Immediate Mode Graphical User Interface):
ImGUI is an abbreviation for "Immediate Mode Graphical User Interface." It is a design approach for creating user interfaces (UI) that allows developers to directly render and manipulate UI elements in real-time. Unlike traditional retained mode GUI systems, where UI elements are created, managed, and updated in a persistent manner, ImGUI focuses on generating the UI each frame based on the current state of the application.

In an ImGUI system, UI elements are typically represented as simple data structures, and the UI state is updated during each frame. This approach makes it efficient and suitable for applications with real-time rendering, such as games and simulations. ImGUI is often used in conjunction with graphics libraries like OpenGL or DirectX to draw UI elements on the screen.

2. SDL (Simple DirectMedia Layer):
SDL is an acronym for "Simple DirectMedia Layer." It is a cross-platform, low-level multimedia library written in C. SDL provides a simple API for managing window creation, user input, audio, and graphics rendering, making it a popular choice for developing games and multimedia applications.

SDL abstracts platform-specific features and provides a consistent interface across various operating systems, making it easier for developers to create cross-platform applications. It supports multiple platforms, including Windows, macOS, Linux, and mobile platforms like iOS and Android.

3. OpenGL (Open Graphics Library):
OpenGL is an open-source graphics rendering API (Application Programming Interface) that allows developers to create high-performance 2D and 3D graphics. It is widely used in the computer graphics industry for rendering complex scenes, visualizations, and games.

OpenGL provides a set of functions for creating and managing graphics objects, rendering primitives (e.g., points, lines, triangles), and applying transformations and shading to achieve various visual effects. It is a low-level API, giving developers fine control over the graphics pipeline and allowing them to optimize performance for specific hardware.

```cpp
#include "header.h"
#include <SDL.h>

// Includes for different OpenGL loaders based on preprocessor directives
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
// ... (Code specific to gl3w loader)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
// ... (Code specific to glew loader)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
// ... (Code specific to glad loader)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
// ... (Code specific to glad2 loader)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Function to create and manage ImGui window for "System" section
void systemWindow(const char *id, ImVec2 size, ImVec2 position)
{
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    // ... (Add content for the "System" window if needed)

    ImGui::End();
}

// Function to create and manage ImGui window for "Memory and Processes" section
void memoryProcessesWindow(const char *id, ImVec2 size, ImVec2 position)
{
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    // ... (Add content for the "Memory and Processes" window if needed)

    ImGui::End();
}

// Function to create and manage ImGui window for "Network" section
void networkWindow(const char *id, ImVec2 size, ImVec2 position)
{
    ImGui::Begin(id);
    ImGui::SetWindowSize(id, size);
    ImGui::SetWindowPos(id, position);

    // ... (Add content for the "Network" window if needed)

    ImGui::End();
}

int main(int, char **)
{
    // Initialize SDL and create a window with an OpenGL context
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Set up SDL attributes for the OpenGL context
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create an SDL window with OpenGL context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    // Initialize the chosen OpenGL loader (gl3w, glew, glad, glbinding2, or glbinding3)
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    // ... (Code specific to glad2 loader, not provided in the snippet)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char *name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
    // ... (Code for custom loader, not provided in the snippet)
#endif

    // Check if the OpenGL loader initialization was successful
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Create ImGui context and set up the color style
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Initialize ImGui with SDL2 and OpenGL
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Set clear color for the OpenGL rendering
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    // Enter the main loop for the GUI application
    bool done = false;
    while (!done)
    {
        // Process SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start a new frame for ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // Draw ImGui windows and contents
        {
            ImVec2 mainDisplay = io.DisplaySize;
            memoryProcessesWindow("== Memory and Processes ==",
                                  ImVec2((mainDisplay.x / 2) - 20, (mainDisplay.y / 2) + 30),
                                  ImVec2((mainDisplay.x / 2) + 10, 10));

            systemWindow("== System ==",
                         ImVec2((mainDisplay.x / 2) - 10, (mainDisplay.y / 2) + 30),
                         ImVec2(10, 10));

            networkWindow("== Network ==",
                          ImVec2(mainDisplay.x - 20, (mainDisplay.y / 2) - 60),
                          ImVec2(10, (mainDisplay.y / 2) + 50));
        }

        // Render ImGui and swap OpenGL buffers
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Clean up ImGui, SDL, and OpenGL resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

```
