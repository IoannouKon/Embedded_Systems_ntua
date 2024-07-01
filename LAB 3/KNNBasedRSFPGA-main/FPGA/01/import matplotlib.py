import matplotlib.pyplot as plt

# Data
categories = ['Simple-float', 'Optimized-float', 'Simple-DTYPE1', 'Optimized-DTYPE1']
clocks_data = [3244037, 429296, 2006021, 392601]

# Plotting
plt.figure(figsize=(10, 6))
plt.bar(categories, clocks_data, color=['blue', 'orange', 'green', 'red'])
plt.xlabel('Categories')
plt.ylabel('Clocks')
plt.title('Clocks Comparison')
plt.show()

import matplotlib.pyplot as plt

# Data
categories_speedup = ['Simple-float', 'Optimized-float', 'Simple-DTYPE1', 'Optimized-DTYPE1']
speedup_data = [0.425847, 3.03361, 0.6826, 3.46912]

# Plotting
plt.figure(figsize=(10, 6))
plt.bar(categories_speedup, speedup_data, color=['blue', 'orange', 'green', 'red'])
plt.xlabel('Categories')
plt.ylabel('Speedup')
plt.title('Speedup Comparison')
plt.show()


import matplotlib.pyplot as plt
import numpy as np

# Data
categories = ['DSP', 'BRAMS', 'LUTs', 'FF']
values1 = [6.25, 35, 1.21, 80]
values2 = [55, 55, 21.67, 60]
values3 = [9.39, 60.34, 12.36, 57.43]
values4 = [3.01, 24.11, 3.23, 12.03]

bar_width = 0.2
index = np.arange(len(categories))

# Plotting
plt.figure(figsize=(10, 6))
plt.bar(index, values1, width=bar_width, label='Simple-float', color='blue')
plt.bar(index + bar_width, values2, width=bar_width, label='Optimized-float', color='orange')
plt.bar(index + 2 * bar_width, values3, width=bar_width, label='Simple-DTYPE1', color='green')
plt.bar(index + 3 * bar_width, values4, width=bar_width, label='Optimized-DTYPE1', color='red')

plt.xlabel('Categories')
plt.ylabel('% Utilazation')
plt.title('')
plt.xticks(index + 1.5 * bar_width, categories)
plt.legend()
plt.show()

