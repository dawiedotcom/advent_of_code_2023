# Day 5: If You Give A Seed A Fertilizer

+ https://adventofcode.com/2023/day/5

This felt like it escalated quickly -- the numbers got really big from the example to real input and Part 2 was much
harder than Part 1.
For Part 1, its possible to keep the maps in hash tables.
However, in Part 2 that approach would require too much memory and you are forced to keep track of intervals instead.

I wasted a lot of time due to a poor representation of intervals, which lead to many edge cases and off by one errors.
In the end a fairly over engineered `interval_` class made the solution much simpler. Hopefully it can be used again in
the future.
