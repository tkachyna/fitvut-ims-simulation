#####################################################################################

#   IMS Project - solar collectors and their efficiency
#  
#   @author Lucia Makaioáv, <xmakai00@stud.fit.vutbr.cz>
#   @date 4/12/2022
#   @file exp3.py
#
#   @brief graph for third experiment

#####################################################################################

import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update({'font.size': 12})

x = [1,2,3,4,5,6,7,8,9,10,11,12]

y_1 = [674, 609, 674, 652,  674,  652,  674,  674, 652, 674, 652, 674] # Q pc potrebne teplo
y_2 = [167, 295, 623, 765, 1050, 1131, 1197, 1094, 831, 490, 183, 105] # Q ku teoreticky vyuzitelne
y_3 = [167, 295, 623, 652,  674,  652,  674,  674, 652, 490, 183, 105] # Q ssu skutecne vyuzitelne

fig = plt.figure(figsize=(10, 5))
ax = fig.add_subplot(1, 1, 1)

ax.plot(x, y_1, label=r"$Q_{p,c} (kWh)$")
ax.plot(x, y_2, label=r"$Q_{k,u} (kWh)$")
ax.plot(x, y_3, label=r"$Q_{ss,u} (kWh)$")
ax.fill_between(x,  y_1,  alpha=0.2)
ax.fill_between(x,  y_2,  alpha=0.2)
ax.fill_between(x,  y_3,  alpha=0.2)

plt.xticks(np.arange(min(x), max(x)+1, 1.0))
ax.legend(loc="upper left")
ax.set_ylabel("kWh")
ax.set_xlabel("Měsíce")
ax.set_ylim(0, 1400)

ax.set_title("Bilance energií (pří sklonu 60°)")

ax.margins(x=0)
ax.margins(y=0)
plt.grid(axis = 'x')
plt.show()