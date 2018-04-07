# plot.py
## plot histogram using data in "plotdata.txt"

import matplotlib.pyplot as plt

fin = open('plotdata.txt','r')

size = int(fin.readline()) 
ranges = []
height = []

for i in range(size):
    ranges.append(float(fin.readline()))
    height.append(float(fin.readline()))

plt.hist(ranges,bins=256,facecolor='r',edgecolor='r',alpha=0.5)
plt.show()
# exit the first histogram to get the second one
plt.hist(height,bins=256,facecolor='b',edgecolor='b',alpha=0.5)
plt.show()

# From the histograms we see that most of the points clique in range 100, and at height from -5 to 5.
# Most of the data points are close and low.