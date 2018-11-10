import matplotlib.pyplot as plt

# x axis values
x1 = [4, 5, 9, 122, 196, 298, 390]
# corresponding y axis values
y1 = [39, 26, 73, 1273, 2027, 632, 3131]

y2 = [120, 114, 156, 917, 1279, 2128, 2155]

# plotting the points
plt.plot(x1, y1, label="vEb", color='green', linestyle='solid', linewidth=3,
         marker='o', markerfacecolor='blue', markersize=12)
plt.plot(x1, y2, label="Binomial Heap", color='red', linestyle='solid', linewidth=3,
         marker='o', markerfacecolor='yellow', markersize=12)

# naming the x axis
plt.xlabel('Number of Vertices')
# naming the y axis
plt.ylabel('Time(in ms)')

# giving a title to my graph
plt.title('vEB vs Binomial Heap')

# function to show the plot
plt.show()
