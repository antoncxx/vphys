import sys

import numpy
import matplotlib.pyplot as plt
import matplotlib.colors as clr
import pylab
import math

if (len(sys.argv) < 2):
	print('Pass a file with calculated values as a cmd line argument\n')
	exit(0)


array = []
with open(sys.argv[1]) as f:
	for line in f:
		sub = []
		for num in line.split():
			sub.append(float(num))
		array.append(sub)



data = numpy.zeros((len(array), len(array[0])))

for index, row in enumerate(array):
	for index2, value in enumerate(row):
		data[index][index2] = value


im = plt.matshow(array, cmap=pylab.cm.hot, norm=clr.LogNorm(vmin=0.01, vmax=64), aspect='auto') # pl is pylab imported a pl
plt.colorbar(im)
plt.show()