import matplotlib.pyplot as plt
from sys import stdin

plt.figure()
color = 'b'
step = 1
i = 0
for line in stdin:
    i += step
    if (not " " in line):
        print()
        color = 'r'
        step = 0
        continue
    a, b = line[1:-2].split(" ")
    a = int(a)
    b = int(b) - 1
    print(i, [a, b], line[1:-2], line, end='')
    plt.plot([a, b], [i, i], color+'o')
    plt.plot([a, b], [i, i], color)

plt.show()
