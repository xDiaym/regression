#!/usr/bin/python3
import numpy as np
import csv

N, a, b = 100, 1.2, 0.3
np.random.seed(42)
noise = np.random.normal(0, 0.1, N)
X = np.linspace(-10, 10, num=N)
Y = a * X + b + noise

with open("dataset.csv", "w") as fp:
    writer = csv.writer(fp)
    writer.writerows(zip(X, Y))
