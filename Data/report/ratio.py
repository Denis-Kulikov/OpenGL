import matplotlib.pyplot as plt

plt.figure(figsize=(9, 7))

with open('lat.txt', 'r') as file1:
    data1 = [list(map(float, line.split())) for line in file1]

with open('data.txt', 'r') as file2:
    data2 = [list(map(float, line.split())) for line in file2]

x_values = [entry[1] / (entry[0] + entry[1]) * 100  for entry in data1]
data = [entry[0] * entry[0] * entry[0] * 8  for entry in data2]

plt.plot(data, x_values, marker='o', linestyle='-')

plt.title('Время работы Exchange')
plt.xlabel('Объём пространства')
plt.ylabel('Доля Exchange в процентах')

plt.savefig('Exchange.png') 

plt.show()
