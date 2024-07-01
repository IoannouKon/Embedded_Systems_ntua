import matplotlib.pyplot as plt

# Given data
total_memory = [136, 134, 352, 608, 384, 168]
cycles = [59348248, 59591662, 41939934, 41934006, 41935983, 41964864]

# Create a scatter plot
plt.scatter(total_memory, cycles, color='red', marker='o')
plt.title('Pareto frontier με γενετικό αλγόριθμο ')
plt.xlabel('Total Memory')
plt.ylabel('Cycles')
plt.grid(True)
plt.show()
