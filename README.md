# LongLife
A pseudo-machine-learning/AI algorithm to generate long-acting Life sets

Conway's Game of Life is a commonly known life simulation system implemented in many ways. After writing up a version of it in C as a code exercise, I began to think about what else I could do going forward from there. Inspired by tom7's Learnfun/Playfun series of machine learning/optimization programs, I sought to create a simple example myself...but what?

This is LongLife: the result of that thought. It began as this concept: **what simple patterns in Life stay alive the longest?**

##Preface
Before we continue, we must consider this question: what is the difference between living and being alive? For now, let us define it as this:

- Living (adj): having life; not dead
- Alive (adj): actively living and changing; not stagnant

With this in mind, we can rule out still-life patterns as living, but not alive. We are looking for patterns with the longest life. There are also known patterns that live forever such as the Gosper glider gun, but we forge ahead as this is a personal exercise in optimization and machine learning, not a serious problem that must be solved.

Note that a pattern may be alive for a long time, but all of the cells might die in the end, making the pattern no longer alive or living. Just as easily, a pattern could only remain alive for a short time before settling into a still-life and remaining living, but no longer alive.

##Theory of Operation
Going back to the main concept, I decided to make the problem easier by breaking it into sections. Let's start with the arbitrary unit "chunk", defined as a small pattern usable in Life. Main.c arbitrarily specifies a chunk as a 5x5 grid, but this technique would just as easily work for a 20000x20000 base chunk. If we pseudo-randomly generate 20 chunks, we can make 160,000 unique patterns, each a 2x2 grid of chunks, which we'll call a "set". Note that a set can be composed of the same chunk repeated four times.

We can then simulate Life using each set and determine the time each set is alive. Taking the theory that a body composed of longer-living parts will live longer than a body composed of shorter-living parts, we can then take the ten sets that remained alive the longest and designate them as new chunks. To add diversity, we also throw in ten randomly generated chunks on the off chance that one of these might be a longer-living chunk than the machine-optimized chunks. Taking these 20 new chunks, we then generate 160,000 new sets and repeat the process until the desired Life pattern size is achieved.

##Current drawbacks
My implementation isn't even working yet (it segfaults upon initialization), but already there are some drawbacks to the method I use to implement it. For example, the 160,000 sets are stored as a contiguous dynamic 3-D array of integers. It would be far better to store these as bits, but the bit datatype isn't even defined in C. I could divide the memory requirement by at least 8 or 16 depending on the bit width of an integer by using bits instead of ints. 

Also, the fact that a contiguous section of memory is needed gets in the way when large memory blocks are requested-the system might not have that much contiguous RAM. A better solution might be to use a tree of linked lists.

Taking the top ten sets out of the 160,000 generated is no small task. I achieved this by creating a 2-D 160,000x2 static array of integers that holds the index of each set in the 3-D dynamic array of sets and its length of time being alive. This array then needs to be sorted by the alive-times from greatest to least, and the indices also need to be kept with their alive-times. Each index acts as a "pseudo-pointer" to where each set is in the dynamic set array. I could have simply sorted the sets themselves, but it's a lot easier and faster (for both me and the computer) to write code to sort a static 2-D 160,000x2 array than a dynamic 3-D array with a size that grows exponentially based off of the size of the Life pattern being sorted.

Even then, I got a bit lazy and didn't bother using a quicksort-I just put in a bubble sort. I have written a quicksort, but that would be a bit annoying to port into the weird memory structure I'm using.

##Compiling
This program is portable and should only depend on the bundled Life.h library and the standard C libraries (stdio, stdlib, etc.) Make sure to compile with command-line output enabled-this doesn't have a GUI.  Beware, the program can take quite a bit of RAM due to the algorithm used. Make sure to compile for 64-bit if possible. I developed this on Windows with TDM-GCC for 32 and 64 bit.

Main.c is the main program; Life.h is a library that handles Life simulation. The files are quite intertwined and built specifically for each other, so they might not be usable in other applications. For example, the Life simulator only returns the lifetime of the pattern given, and the pattern is given to it as an integer selecting a pattern out of a dynamically allocated 3 dimensional array passed as pointer to a 1 dimensional array accessed as a 3 dimensional array using a custom macro and some math (whew!). 
