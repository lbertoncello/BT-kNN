import sys
import numpy as np 
from scipy import spatial

def write_matrix_as_csv(matrix, output_file):
    with open(output_file, 'w') as f:
        for line in matrix:
            f.write("%s\n" % str(" ".join(map(str, line.astype(np.float)))))

def write_matrix_as_matrix_market(matrix, output_file):
    with open(output_file, 'w') as f:
        f.write("%%Matrix Market\n")
        f.write("%d %d %d\n" % (matrix.shape[0], matrix.shape[1], matrix.shape[0] * matrix.shape[1]))

        for i in range(matrix.shape[0]):
            for j in range(matrix.shape[1]):
                f.write("%d %d %f\n" % (i+1, j+1, matrix[i][j]))


dataset_file = sys.argv[1]
output_file = sys.argv[2]

with open(dataset_file, 'r') as f:
    lines = f.readlines()

arrays = np.zeros([len(lines), len(lines)])

for i in range(len(lines)):
    for j in range(len(lines)):
        u = np.array([float(e) for e in lines[i].replace('\n', '').split(',')]).reshape(1, -1)
        v = np.array([float(e) for e in lines[j].replace('\n', '').split(',')]).reshape(1, -1)

        arrays[i][j] = 1 - spatial.distance.cosine(u, v)

write_matrix_as_matrix_market(arrays, output_file)

