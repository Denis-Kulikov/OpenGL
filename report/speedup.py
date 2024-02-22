import matplotlib.pyplot as plt
import numpy as np

plt.figure(figsize=(9, 8))

def plot_speedup(file_path, color, label):
    with open(file_path, 'r') as file:
        data = [line.strip().split('\t') for line in file]
        bodies = [int(part[0]) for part in data]
        times = [float(part[1]) for part in data]

    speedup = [times[0] / times[i] for i in range(1, len(times))]

    measurements = [bodies[i] for i in range(1, len(times))]

    plt.plot(measurements, speedup, label=label, color=color, marker='o')

plt.title('Ускорение')

plot_speedup('nbody-4287.txt', color='blue', label='Leapfrog 4287 тел')

plt.legend()
plt.xlabel('Число процессов')
plt.ylabel('Ускорение')

plt.grid(True)

plt.savefig('speedup.png')

plt.show()
