import matplotlib.pyplot as plt
import numpy as np

# Given data points
x = [1, 2, 3, 4, 5, 6 ,8]
y = [153601, 12291,8187,6147,4899,4889,3075 ] #Interval
#y=[153601,14341,10237,8197,6949,6949,5125]   #latency
# Create a range of x-values for the continuous function
x_continuous = np.linspace(min(x), max(x), 100)

# Interpolate the function using numpy's interp
y_continuous = np.interp(x_continuous, x, y)

# Plot the data points as a scatter plot with customizations
plt.figure(figsize=(8, 6))  # Set the size of the figure
plt.scatter(x, y, color='red', label='Data Points', marker='o', s=80)  # Use circles for markers

# Plot the continuous function as a line
plt.plot(x_continuous, y_continuous, label='Continuous Function', linestyle='--', linewidth=2)

plt.xlabel('instances', fontsize=14)  # Set the label and its font size for the x-axis
plt.ylabel('latency', fontsize=14)  # Set the label and its font size for the y-axis
plt.title('Plot of latency with instances', fontsize=16)  # Set the title and its font size
plt.grid(True, linestyle='--', alpha=0.7)  # Add grid lines with a dashed style and reduced opacity
plt.legend(fontsize=12)  # Set the legend font size

# Customizing the tick labels on both axes
plt.xticks(x, fontsize=12)  # Set custom x-axis tick labels using the values in the 'x' list
plt.yticks(fontsize=12)  # Set the font size of y-axis tick labels

# Adding a background color to the plot
ax = plt.gca()
ax.set_facecolor('#f0f0f0')  # Choose any color you like

# Adding annotations for each point with the values
for i, txt in enumerate(y):
    plt.annotate(txt, (x[i], y[i]), textcoords="offset points", xytext=(5, 5), ha='center', fontsize=10)
    plt.annotate(f"({x[i]}, {y[i]})", (x[i], y[i]), textcoords="offset points", xytext=(-15, -20), ha='center', fontsize=8, color='blue')

# Display the plot (optional, you can save it to a file if you prefer)
plt.tight_layout()  # Adjust spacing for a cleaner look
plt.show()



#eg 
# i=1 BRAM=0 DSP=29 FF = 8 LUT =30
# i=2 BRAM=54 DSP=59 FF =20 LUT =72
# i=3 BRAM=40 DSP=90 FF =31 LUT =108
# i=4 BRAM=54 DSP=120 FF =41 LUT =141
# i=5 BRAM=67 DSP=150 FF =52 LUT =178
# i=6 BRAM=81 DSP=181 FF =63 LUT =214
# i=7 BRAM=96 DSP=211 FF =73 LUT =249

import matplotlib.pyplot as plt

# Given data
i_values = [1, 2, 3, 4, 5, 6, 8]
BRAM_values = [66, 8, 12, 16, 20, 24, 32]
DSP_values = [5, 10, 15, 20, 25, 30, 40]
FF_values = [3125, 2850, 3808, 4720, 5768, 7747, 8489]
LUT_values = [9683, 9661, 12276, 13748, 15429, 16229, 17783]

# Create the bar plot
plt.figure(figsize=(10, 6))  # Set the size of the figure
bar_width = 0.2  # Set the width of the bars

# Shift the x positions for each set of bars to prevent overlap
bar_positions_i = [i for i in range(len(i_values))]
bar_positions_BRAM = [pos + bar_width for pos in bar_positions_i]
bar_positions_DSP = [pos + 2 * bar_width for pos in bar_positions_i]
bar_positions_FF = [pos + 3 * bar_width for pos in bar_positions_i]

# Plot the bars for each value
plt.bar(bar_positions_i, BRAM_values, bar_width, label='BRAM')
plt.bar(bar_positions_DSP, DSP_values, bar_width, label='DSP')
plt.bar(bar_positions_FF, FF_values, bar_width, label='FF')
plt.bar(bar_positions_FF, LUT_values, bar_width, label='LUT', alpha=0.7)  # Add LUT bars with reduced opacity

plt.xlabel('instances', fontsize=14)  # Set the label and its font size for the x-axis
plt.ylabel('Utilization %', fontsize=14)  # Set the label and its font size for the y-axis
plt.title('Resource Utilization % for Different instances Values', fontsize=16)  # Set the title and its font size
plt.xticks(bar_positions_i, i_values)  # Set custom x-axis tick labels using the 'i_values' list
plt.legend(fontsize=12)  # Set the legend font size

# Display the plot (optional, you can save it to a file if you prefer)
plt.tight_layout()  # Adjust spacing for a cleaner look
plt.show()