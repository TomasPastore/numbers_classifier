# -*- coding: UTF-8 -*-
import numpy as np
import matplotlib.pyplot as plt
#La matriz tiene que estar guardada en el archivo que diga ahí con como en matlab, onda: 1,0,0;0,1,0;0,0,1 es la matriz I
#Y bueno, supongo que tienen numpy y matplotlib, si no "pip install numpy" y "pip install matplotlib", por ahi con sudo
conf_arr = []
with open("confusion.txt",'r') as f:
	line = f.read()
	for l in line.split(';'):
		conf_arr.append([int(n) for n in l.split(',')])
         

norm_conf = []
for i in conf_arr:
    a = 0
    tmp_arr = []
    a = sum(i, 0)
    for j in i:
        tmp_arr.append(float(j)/float(a))
    norm_conf.append(tmp_arr)

fig = plt.figure()
plt.clf()
ax = fig.add_subplot(111)
ax.set_aspect(1)
ax.set_xlabel(u'Predicción')
ax.set_ylabel('Etiqueta')

res = ax.imshow(np.array(norm_conf), cmap=plt.cm.jet, 
                interpolation='nearest')

width, height = np.array(conf_arr).shape

for x in xrange(width):
    for y in xrange(height):
        ax.annotate(str(conf_arr[x][y]), xy=(y, x), 
                    horizontalalignment='center',
                    verticalalignment='center')

cb = fig.colorbar(res)
alphabet = '0123456789'
plt.xticks(range(width), alphabet[:width])
plt.yticks(range(height), alphabet[:height])
plt.savefig('confusion_matrix.png', format='png')
plt.show()