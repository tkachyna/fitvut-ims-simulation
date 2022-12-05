#####################################################################################

#   IMS Project - solar collectors and their efficiency
#  
#   @author Lucia Makaioáv, <xmakai00@stud.fit.vutbr.cz>
#   @date 4/12/2022
#   @file exp1.py
#
#   @brief graph for first experiment

#####################################################################################

import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update({'font.size': 15})

x = [1,2,3,4,5,6,7,8,9,10,11,12]

y_1 = [164,273,552,659,889,950,1003,919,706,428,160,103]
y_2 = [131,227,415,500,638,697,724,679,559,345,152,90]

fig = plt.figure(figsize=(10, 5))
ax = fig.add_subplot(1, 1, 1)

ax.plot(x, y_1, label="náš model")
ax.plot(x, y_2, label="reference")
ax.fill_between(x,  y_1,  alpha=0.2)
ax.fill_between(x,  y_2,  alpha=0.2)

plt.xticks(np.arange(min(x), max(x)+1, 1.0))
ax.legend(loc="upper left")
ax.set_ylabel(r"$Q_{k,u} (kWh)$")
ax.set_xlabel("Měsíce")
ax.set_ylim(0, 1200)
ax.set_title("Teoreticky využitelné tepelné zisky " + r"$Q_{k,u} (kWh)$")

ax.margins(x=0)
ax.margins(y=0)

plt.show()