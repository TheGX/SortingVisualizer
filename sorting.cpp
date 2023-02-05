// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "Includes/imgui.h"
#include "Includes/imgui_impl_glfw.h"
#include "Includes/imgui_impl_opengl3.h"
#include "Includes/implot.h"
#include <cstring>
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
static ImVec4 PLOT_CURSOR_COLOR = {1.f, 0.f, 0.f, 1.f};
static ImVec4 ImVec4Orange = {1.0f,0.621,0.010,0.784};
static ImVec4 ImVec4Green = {0.034,1.0f,0.120,0.784};


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
    ImPlot::StyleColorsDark();
    


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    static bool sorted = false;
    static bool cont = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    int sizeArrayToSort = 10;

    Vector randomInts = {
        .data = (int *)malloc(sizeof(int)*10), 
        .capacity = 10, 
        .size = 0
    };
    
    Vector sortedValues = {
        .data = (int *)malloc(sizeof(int)*10), 
        .capacity = 10, 
        .size = 0
    };

    bool stdSorted; 
            
    static coroutine_t s_co;
    coroutine_t* co = &s_co;

    generateNRandomInts(&randomInts, randomInts.capacity);
    //"Reset" array to be sorted back to random
    //copyArryIntToFloat((int*)randomInts.data, randomInts.size, sortedFloats);

    Vector* pSortedValues = &sortedValues;
    copyVector(&randomInts, pSortedValues);

    const char* sortingAlgos[] = { "Selection Sort", "Buble Sort", "Quick Sort", "Heap Sort", "Insertion Sort", "Merge Sort"};
    // { "BitonicSort", "BogoSort", "CocktailSort", "CombSort", "GnomeSort", 
        //"MergeSort", "PancakeSort", "RadixSort (LSD)", "RadixSort (MSD)",
        //"ShellSort", "StalinSort" };

    sortingFunction funcs[] = { selectionSort, bubleSort, quickSort, heapSort, insertionSort, mergeSort};
    std::unordered_map<const char*, sortingFunction> sortingFunctionsMap;

    // Build Sorting Function Map
    for( int sortingAlgoName = 0; 
            sortingAlgoName < _countOf(sortingAlgos) && sortingAlgoName < _countOf(funcs);
            sortingAlgoName++)
    {
        sortingFunctionsMap.emplace(sortingAlgos[sortingAlgoName], funcs[sortingAlgoName]);
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

            ImGui::PushItemWidth(126);

            // Algorithm drop Menu
            ImGui::Text("Sorting Algorithms:");
            ImGui::SameLine();
            
            static int currentAlgo = 0;
            if (ImGui::Combo("##Sorting Algo", &currentAlgo, sortingAlgos, IM_ARRAYSIZE(sortingAlgos))) 
            {
                // Reset array to replay animation
                copyVector(&randomInts, pSortedValues);
                sorted = false;
            }
            
            ImGui::SameLine();
            ImGui::Text("Plot Styling:");
            ImGui::SameLine();

            ImPlotContext &gp = *ImPlot::GetCurrentContext();
            if (ImGui::BeginCombo("##PlotStyling", gp.ColormapData.GetName(gp.Style.Colormap))) {
                for (int i = 0; i < gp.ColormapData.Count; ++i) {
                    const char* name = gp.ColormapData.GetName(i);
                    if (ImGui::Selectable(name, gp.Style.Colormap == i)) {
                        gp.Style.Colormap = i;
                        ImPlot::BustItemCache();
                    }
                }
                ImGui::EndCombo();
            }       
            
            ImGui::SameLine();
            stdSorted = std::is_sorted((int*)sortedValues.data, (int*)sortedValues.data + sortedValues.size);
            stdSorted ? ImGui::TextColored(ImVec4Green, "Sorted: true") :
                ImGui::TextColored(ImVec4Orange, "Sorted: false");
            
            ImGui::PopItemWidth();
            
            ImGui::PushItemWidth(252);
            ImGui::Text("Number of elements");
            
            ImGui::SameLine();
            if (ImGui::SliderInt("##NumberOfElements", (int*)&randomInts.capacity, 1, 500))
            {
                updateSequence(&randomInts, pSortedValues, co, sorted);
            }
            
            ImGui::SameLine();
            if (ImGui::Button("New Random Sequence")) 
            {
                updateSequence(&randomInts, pSortedValues, co, sorted);
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
            //Reset iterateOnce and pause the coroutine if true
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
            
            ImGui::PopItemWidth();
            
            // Plot random array
            static ImPlotAxisFlags xflags = ImPlotAxisFlags_AutoFit ; 
            static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit ; 

            if( ImPlot::BeginPlot("Random Sequence")) 
            {
                ImPlot::SetupAxes("", "Random Sequence", xflags, yflags);
                ImPlot::PlotBars("##Random_Sequence", (int*)randomInts.data, randomInts.size, 0.67);
                ImPlot::EndPlot();
            }
            
            if(sorted == false && co->paused != true) 
            {
                // Find current Sorting Algorithm
                if( sortingFunctionsMap.find(sortingAlgos[currentAlgo]) != sortingFunctionsMap.end() )
                {
                        sortingFunction algorithmSorter =
                            sortingFunctionsMap.find(sortingAlgos[currentAlgo])->second;
                        sorted = algorithmSorter((int*)sortedValues.data, sortedValues.size, co);
                }
            }
            
            char plotTitle[30] = "Algorithm: ";
            strcat(plotTitle, sortingAlgos[currentAlgo]);
            if( ImPlot::BeginPlot(plotTitle) ) 
            {
                // Plot current sorted array
                ImPlot::SetupAxes("", "Random Sequence", xflags, yflags);
                ImPlot::PlotBars(sortingAlgos[currentAlgo], (int*)sortedValues.data, sortedValues.size, 0.67);

                //Highlight cursor Position onto the plot
                ImPlot::PushStyleColor(ImPlotCol_Fill, PLOT_CURSOR_COLOR);
                //unsigned int cursorValue = (sortedFloats[co->cursorPosition]);
                unsigned int cursorValue = (((int*)sortedValues.data)[co->cursorPosition]);
                ImPlot::PlotBars("##Cursor", &(co->cursorPosition), &cursorValue, 1, 0.67f);
                ImPlot::PopStyleColor();

                ImPlot::EndPlot();
            }
            
            // TODO:Place this in a better place
            // Activate Coroutine
            co->active = true;

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

    freeVector(&randomInts);
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
