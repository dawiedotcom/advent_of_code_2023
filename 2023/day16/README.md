# Day 16: The Floor Will Be Lava

* https://adventofcode.com/2023/day/16

## Objects on stacks

A memory bug is caused when using a reference to the top of the stack and pushing to the stack
while still using the reference:
```c++
beam_t& beam = beams.top();
beams.pop();
//...
beams.push(...); // the new beams is in the same slot as beam, the old beams.top()
// other code that uses beam
```

When items will be pushed to the stack while the old top is manipulated, a
copy of the top element should be created instead:
```c++
beam_t beam(beams.top()); // creates a new object separate from the top of the stack
beams.pop();
//...
beams.push(...); // safe to still use beam
```
