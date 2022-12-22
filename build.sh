if [ ! -d "build/" ]
then
    mkdir build
    cc -O2 -g3 Includes/raylib/src/rglfw.c -c -o build/rglfw.o
    c++ -O2 -g3  -I Includes/ Includes/imgui.cpp -c -o build/imgui.o
    c++ -O2  -g3 -I Includes/ Includes/imgui_draw.cpp -c -o build/imgui_draw.o
    c++ -O2  -g3 -I Includes/ Includes/imgui_widgets.cpp -c -o build/imgui_widgets.o
    c++ -O2  -g3 -I Includes/ Includes/imgui_tables.cpp -c -o build/imgui_tables.o
    c++ -O2  -g3 -I Includes -I ./ backends/imgui_impl_glfw.cpp -c -o build/imgui_impl_glfw.o
    c++ -O2  -g3 -I Includes -I ./ backends/imgui_impl_opengl3.cpp -c -o build/imgui_impl_opengl3.o
fi
#c++ -I ./ -I build/ -I backends/  sorting.cpp implot/implot_demo.cpp imgui_demo.cpp build/*.o -o build/foo -g3 -lGL -lX11 -ldl -lpthread 
c++ -I ./ -I build/ -I backends/  sorting.cpp  Includes/imgui_demo.cpp build/*.o -o build/foo -g3 -lGL -lX11 -ldl -lpthread  -Wfatal-errors

export DISPLAY=$(ipconfig.exe | grep IPv4 | head -1 | tr -d '\r' | awk '{print $NF}'):0.0
export LIBGL_ALWAYS_INDIRECT=1


