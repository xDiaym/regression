#!/usr/bin/python3
import argparse
import csv
import sys

import numpy as np


def main(N: int, a: float, b: float, seed: int):
    np.random.seed(seed)
    noise = np.random.normal(0, 0.1, N)
    X = np.linspace(-10, 10, num=N)
    Y = a * X + b + noise

    with open("dataset.csv", "w") as fp:
        writer = csv.writer(fp)
        writer.writerows(zip(X, Y))
    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate dataset with noise.")
    parser.add_argument("-N", type=int, default=10_000)
    parser.add_argument("-a", type=float, default=1, help="Slope of a line.")
    parser.add_argument("-b", type=float, default=2, help="Y-intercept point.")
    parser.add_argument("--seed", type=int, default=42, help="PRNG seed.")
    args = parser.parse_args()
    sys.exit(main(args.N, args.a, args.b, args.seed))
