# SortingVisualizer
The main logic behind this was to implement one of those sorting visualizers
you see on gifs that show how the different sorting algorithms work.

I decided to use ImGui (and ImPlot) and a very simple 
[Coroutine implementation](https://github.com/RandyGaul/kk_slides) I had stumble upon.  
The use of Coroutines seemed adequate here given the general flowchart I had in mind for a sorting visualizer:

- Do 1 iteration of the current sorting algorithm;
- "Pause" this iteration process;
- Render the current state of the sorting array, highlighting the current element being compared;
- Repeat until array is sorted;

With this logic in mind, the choice for ImGui and ImPlot for rendering the UI
seemed obvious since they render the screen every frame.

## Approach problems:
Soon after I start implementing this approach, I understood some general problems.

(Normaly I would change the approach I'm using to avoid these types of big scope pproblems all together.
This time however, it seemed fitting to follow through all the way and try to see what the end result). 

### Given the Coroutine implementation used, recursion became a nightmare. 

Basically the coroutine implementation does the following:

```c
do { 
    switch (co->index) 
    { 
        default: //This comes from COROUTINE_START(co)

        case __LINE__: 
           <name>: co->index = __LINE__;
        ....

        goto __co_end; //This comes from COROUTINE_EXIT(co)
    } 

    co->index = 0; 
    __co_end:;  //This comes from COROUTINE_END(co)
} while (0)
```

By storing the __LINE__ where execution was last left off (before exiting the
switch case statement) in the coroutine_t struct co->index variable, you can then pick
back up the next time you pass on through.

The problem with this logic inside recursion based algorithms, is that once you
"pause" the sorting algorithm after 1 iteration and then come back, you have no
clear way of knowing at which point of the recursion you entered the switch.

- "Solution": Iterative implementations of the sorting algorithms. Not
  necessarly bad by itself, but leaves some implementations a lot messier.

### "Code styling" specially regarding static variables.

-----------------------------------
##TODO: 
 - [x] Dynamic array sizes;
 - [x] Add step through option;
 - [x] Dynamic Coroutine wait time;
 - Add pivot to IMPLOT in special algos;
