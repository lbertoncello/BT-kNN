import sys
import numpy as np

def read_mtx(input_file_name):
    input_file = open(input_file_name, 'r')
    lines = input_file.readlines()
    input_file.close()

    num_lines, num_columns, elements = lines[1].split(' ')

    matrix = np.zeros([int(num_lines), int(num_columns)])

    for k in range(2, len(lines)):
        i, j, element = lines[k].split(' ') 
        matrix[int(i) - 1][int(j) - 1] = float(element)

    return matrix

def write_csv(matrix, output_file_name, delimiter):
    with open(output_file_name, 'w') as f:
	    for line in matrix:
		    f.write("%s\n" % str(delimiter.join(map(str, line))))

def main():
    input_file_name = sys.argv[1]
    output_file_name = sys.argv[2]
    delimiter = sys.argv[3]

    matrix = read_mtx(input_file_name)

    write_csv(matrix, output_file_name, delimiter)

if __name__ == "__main__":
    main()