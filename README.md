# SortingVisualizer
The main logic behind this was to implement one of those sorting visualizers
you see in gifs to explain the different sorting algorithms.

I decided to use ImGui and ImPlot and a very simple 
[Coroutine implementation](https://github.com/RandyGaul/kk_slides) I had stumble upon.  
The use of Coroutines seemed adequate here given the general flowchart I had in mind for a sorting visualizer:

- Do 1 iteration of the current sorting algorithm;
- Pause this iteration process;
- Render the current state of the sorting array, highlighting the current element being compared;
- Resume the sorting algorithm;
- Repeat until array is sorted;

With this logic in mind, the choice for ImGui and ImPlot for rendering the UI
seemed obvious since they render the screen every frame.

## Approach problems:
Soon after I start implementing this approach, I understood some general problems.

### Given the Coroutine implementation used, recursion became a nightmare. 
How are you going 
- "Solution": Iterative implementations;

### "Code styling" specially regarding static variables.

##TODO: 
 - [x] Dynamic array sizes;
 - [x] Add step through option;
 - [x] Dynamic Coroutine wait time;
 - Add pivot to IMPLOT in special algos;
