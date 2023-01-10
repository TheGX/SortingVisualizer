// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "Includes/imgui.h"
#include "Includes/imgui_impl_glfw.h"
#include "Includes/imgui_impl_opengl3.h"
#include "Includes/implot.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "Includes/implot_internal.h"
#include "Includes/implot.cpp"
#include "Includes/implot_items.cpp"
#include "algos.cpp"
#include "algorithm"
#include "unordered_map"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


typedef bool (*sortingFunction) (int* arrayToSort, int count, coroutine_t* co);
    
static ImVec4 PLOT_BARS_NORMAL_COLOR = {0.2f, 0.41f, 0.69f, 1.f};
static ImVec4 PLOT_BARS_CURSOR_COLOR = {1.f, 0.f, 0.f, 1.f};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1900, 900, "Sorting algos", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    static bool sorted = false;
    static bool cont = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float randomFloats[_countOf(randomInts)];
    int arrayToSort[_countOf(randomInts)];
    float sortedFloats[_countOf(randomFloats)];
    bool stdSorted; 
            
    static coroutine_t s_co;
    coroutine_t* co = &s_co;

    getNRandomInts(_countOf(randomInts));
    //"Reset" array to be sorted back to random
    copyArryInt(randomInts, _countOf(randomInts), arrayToSort); 
    copyArryIntToFloat(randomInts, _countOf(randomInts), randomFloats);


    const char* sortingAlgos[] = { "Selection Sort", "Buble Sort", "Quick Sort", "Heap Sort", "Insertion Sort", "Merge Sort"};
    //inline static const char* algorithmsNames[] = { "BitonicSort", "BogoSort", "CocktailSort", "CombSort", "GnomeSort", 
        //"MergeSort", "PancakeSort", "RadixSort (LSD)", "RadixSort (MSD)", "ShellSort", "StalinSort" };

    sortingFunction funcs[] = { selectionSort, bubleSort, quickSort, heapSort};
    std::unordered_map<const char*, sortingFunction> map;

    for( int sortingAlgoName = 0; 
            sortingAlgoName < _countOf(sortingAlgos) && sortingAlgoName < _countOf(funcs);
            sortingAlgoName++)
    {
        map.emplace(sortingAlgos[sortingAlgoName], funcs[sortingAlgoName]);
    }
            
    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow(&show_demo_window);
        
        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(1729, 713),ImGuiCond_Always);
            ImGui::Begin("Sorting Algorithm Window", &show_another_window);   
            
            // Algorithm drop Menu
            ImGui::Text("Sort Algos");
            ImGui::SameLine();
            
            static int currentAlgo = 0;
            if (ImGui::Combo("##Sorting Algo", &currentAlgo, sortingAlgos, IM_ARRAYSIZE(sortingAlgos))) {
                
                // Reset array to replay animation
                copyArryInt(randomInts, _countOf(randomInts), arrayToSort); 
                copyArryIntToFloat(randomInts, _countOf(randomInts), randomFloats);
                sorted = false;
            }
            
            ImGui::Text("Coroutine Speed time");
            ImGui::SameLine();
            ImGui::SliderFloat("##Coroutine_dt", &co->dt, 1, 5);

            ImGui::SameLine();
            if (co->paused == false) 
            {
                if (ImGui::Button("Pause Coroutine")) 
                {
                    co->paused = true;
                }
            }
            else if (ImGui::Button("Unpause Coroutine")) 
            {
                co->paused = false;
            }
            
            static bool iterateOnce = false;
            //Reset iterateOnce and pause the coroutine
            if (iterateOnce == true) 
            {
                co->paused = true;
                iterateOnce = false;
            }
            
            if (co->paused == true) 
            {
                ImGui::SameLine();
                //Unpause coroutine and mark iterateOnce
                if (ImGui::Button("Iterate once")) 
                {
                    iterateOnce = true;
                    co->paused = false;
                    co->dt = co->time-1;
                }

            }
            
            ImGui::SameLine();
            if (ImGui::Button("New Random Sequence")) 
            {
                //TODO: randomInts should not be global, if it is then no need for the size as arg
                getNRandomInts(_countOf(randomInts));
           
                //"Reset" array to be sorted back to random
                copyArryInt(randomInts, _countOf(randomInts), arrayToSort); 
                copyArryIntToFloat(randomInts, _countOf(randomInts), randomFloats);
                sorted = false;
            }
            
            // Plot current sorted array
            if( ImPlot::BeginPlot("Random Sequence")) {
                ImPlot::PushStyleColor(ImPlotCol_Fill, PLOT_BARS_NORMAL_COLOR );
                ImPlot::PlotBars("##Random_Sequence", randomFloats, _countOf(randomFloats), 0.67);
                ImPlot::PopStyleColor();
                ImPlot::EndPlot();
            }
            
            if(sorted != true && co->paused != true) 
            {
                // Find current Sorting Algorithm
                if( map.find(sortingAlgos[currentAlgo]) != map.end() )
                {
                        sortingFunction algorithmSorter = map.find(sortingAlgos[currentAlgo])->second;
                        sorted = algorithmSorter(arrayToSort, _countOf(arrayToSort), co);
                }

                
                // Copy current state of sorted array to be displayed 
                copyArryIntToFloat(arrayToSort, _countOf(arrayToSort), sortedFloats);
            }
            
            char plotTitle[30] = "Algorithm: ";
            strcat(plotTitle, sortingAlgos[currentAlgo]);
            if( ImPlot::BeginPlot(plotTitle) ) {

                // Plot current sorted array
                ImPlot::PushStyleColor(ImPlotCol_Fill, PLOT_BARS_NORMAL_COLOR );
                ImPlot::PlotBars(sortingAlgos[currentAlgo], sortedFloats, _countOf(sortedFloats), 0.67);
                ImPlot::PopStyleColor();

                //Highlight cursor Position onto the plot
                ImPlot::PushStyleColor(ImPlotCol_Fill, PLOT_BARS_CURSOR_COLOR);
                unsigned int cursorValue = (sortedFloats[co->cursorPosition]);
                ImPlot::PlotBars("##Cursor", &(co->cursorPosition), &cursorValue, 1, 0.67f);
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }

            stdSorted = std::is_sorted(std::begin(arrayToSort), std::end(arrayToSort));
            stdSorted ? ImGui::TextColored(ImVec4(0.034,1.0f,0.120,0.784), "Sorted: true") :
                ImGui::TextColored(ImVec4(1.0f,0.621,0.010,0.784), "Sorted: false");
            
            ImGui::End();
        }
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y *
                clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
