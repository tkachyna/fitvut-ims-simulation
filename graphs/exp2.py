#####################################################################################

#   IMS Project - solar collectors and their efficiency
#  
#   @author Lucia Makaioáv, <xmakai00@stud.fit.vutbr.cz>
#   @date 4/12/2022
#   @file exp2.py
#
#   @brief graph for second experiment

#####################################################################################

import matplotlib.pyplot as plt
import numpy as np

X =['Oranžová', 'Červená', 'Modrá', 'Žlutá']

from pylab import rcParams
rcParams['figure.figsize'] = 10, 4

plt.rcParams.update({'font.size': 15})

slope45 = [1801, 1801, 1851, 0]
slope60 = [1666, 1666, 1726, 0]
slope90 = [1092, 1092, 1193, 0]

X_axis = np.arange(len(X))

plt.bar(X_axis + 0, slope45, 0.25, label = '45°')
plt.bar(X_axis + 0.25, slope60, 0.25, label = '60°')
plt.bar(X_axis + 0.50, slope90, 0.25, label = '90°')
 
plt.xticks(X_axis+ 0.25, X)
plt.legend()
plt.ylabel("kWh")
plt.grid(axis = 'y')
plt.xlabel("Část střechy")
plt.title("Energie vyrobena kolektorem CosmoSOL za jeden rok (kWh)")
plt.show()