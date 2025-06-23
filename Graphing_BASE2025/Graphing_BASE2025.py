
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

file = pd.read_csv("ROOF0618.csv")

dataTag = file["dataTag"].to_numpy()

middle = file["Middle"].to_numpy()
side = file["Side"].to_numpy()
top = file["Top"].to_numpy()
hori = file["Horizontal"].to_numpy()
verti = file["Vertical"].to_numpy()


plt.plot(dataTag,middle, label = "Middle")
plt.plot(dataTag,side, label = "Side")
plt.plot(dataTag,top, label = "Top")
plt.plot(dataTag,hori, label = "Horizontal")
plt.plot(dataTag,verti, label = "Vertical")

plt.xlabel("Halves of minute")
plt.ylabel("Counts")


plt.legend(loc='upper left')
plt.show()



