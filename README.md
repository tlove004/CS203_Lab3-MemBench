STREAM was modified to incorporate the memory latency benchmark from lmbench3 (lat_mem_rd), available at: http://www.bitmover.com/lmbench/get_lmbench.html

The libraries were modified to support Windows, macOS, and Linux, as the original code did not compile on non-Linux systems.   To compile the benchmark, use:

`gcc stream2.c lib_mem.c -o benchmark`

The executable takes two arguments, a size (in MB, at least 4 times the size of last level cache) and a stride (should be >= cache line size, to mitigate prefetching optimizations).  

Run the compiled ‘benchmark’ file as:

`./benchmark <size> <stride>`

For example, an x86 system with a 64 Byte cache line size, with 32KB L1$, 256KB L2$ and 8MB L3$, you could run:

`./benchmark 24 256`

The STREAM benchmark will output first, followed by the lat_mem_rd results.  Two columns are output for each memory latency read; the first outputs the size of the array (in KB), the second is the latency to read the array (in ns).   For the above system, you can clearly see a change when a cache is unable to fully read the array.  For example, when filling the L1$ and the L2$, we see a change from ~2.7ns(L1$) to ~4.6ns(L2$) and a change from ~4.6ns(L2$) to ~9.5ns(L3$):

………
26.00000  	 2.710
28.00000  	 2.721
30.00000  	 2.707
32.00000  	 2.702
36.00000  	 4.606
40.00000  	 4.599
44.00000  	 4.613
……..
208.00000  	 4.600
224.00000  	 4.403
240.00000  	 4.739
256.00000  	 4.855
288.00000  	 9.581
320.00000  	 9.541
352.00000  	 9.352
………

