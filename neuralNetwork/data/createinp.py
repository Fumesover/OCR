import sys
import argparse
import os

parser = argparse.ArgumentParser(description='Take .csv and convert it to .inp')
parser.add_argument('-i', help='input file', required=True)
#parser.add_argument('-o', help='output file', required=True)
parser.add_argument('--verbose', '-v', action='count')

args = parser.parse_args()

i = open(args.i, 'r')
i.readline()
o = open(args.i.split('.')[0]+'.inp', 'w')
#o = open(args.o, 'w')

num_lines = sum(1 for line in open(args.i))

print(num_lines - 1, 400, 127 - 33, file=o)

for l in i:
    l = l.split(',')
    for i in range(20):
        for j in range(20):
            c = int(l[i * 20 + j + 12]) / 255
            print(c, file=o, end=' ')

    value = int(l[2])
    
    if args.verbose and args.verbose > 0:
        print("value =", value, "(", chr(value), ")")

    for _ in range(33, value):
        print("0 ", file=o, end="")
    print("1 ", file=o, end="")
    for _ in range(value + 1, 127):
        print("0 ", file=o, end="")

    print("", file=o)
