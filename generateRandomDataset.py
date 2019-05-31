import sys
import numpy as np
import os
import struct


def generate_dataset(num_of_features, output_file):
    output = ""

    for i in range(num_of_features):
        output += "%d\n" % (i)

    with open(output_file, 'w') as f:
        f.write(output)


def generate_classes(num_of_features, output_file):
    output = ""

    num_of_classes = 5
    classes = []

    for i in range(num_of_classes):
        classes.append("class%d\n" % (i))

    for i in range(num_of_features):
        output += classes[np.random.random_integers(0, 4)]

    with open(output_file, 'w') as f:
        f.write(output)


def generate_similarities(num_of_features, output_file):
    num_of_rows = num_of_features
    num_of_cols = num_of_features
    num_of_elems = num_of_features * num_of_features

    similarities = np.random.rand(num_of_rows, num_of_cols)

    with open(output_file, 'wb') as f:
        f.write(struct.pack('@i', int(num_of_rows)))
        f.write(struct.pack('@i', int(num_of_cols)))
        f.write(struct.pack('@i', int(num_of_elems)))

        for line in similarities:
            for element in line:
                f.write(struct.pack('@f', element))


def generate_k(k_values, output_file):
    with open(output_file, 'w') as f:
        for k in k_values:
            f.write("%d\n" % (k))


def generate_run(command, output_file):
    with open(output_file, 'w') as f:
        f.write("%s\n" % (command))


def main():
    num_of_features=int(sys.argv[1])

    k_values=[7]

    dataset_dir="./Random_datasets/dataset%d" % (
        num_of_features)
    dataset_filename="%s/dataset.txt" % (
        dataset_dir)
    classes_filename="%s/classes.txt" % (
        dataset_dir)
    k_filename="%s/k.txt" % (
        dataset_dir)
    binary_similarities_filename="%s/similarities.bin" % (
        dataset_dir)
    subsets_dirname="%s/Subsets" % (dataset_dir)
    run_filename="%s/run.txt" % (
        dataset_dir)

    os.mkdir(dataset_dir)
    os.mkdir(subsets_dirname)
    os.mkdir("%s/Experiments" % (dataset_dir))
    os.mkdir("%s/Experiments/Tree" %
             (dataset_dir))

    generate_dataset(num_of_features, dataset_filename)
    generate_classes(num_of_features, classes_filename)
    generate_k(k_values, k_filename)
    generate_similarities(num_of_features, binary_similarities_filename)

    os.system("python3 create_subsets.py %s %s %s" %
              (dataset_filename, classes_filename, subsets_dirname))

    number_of_nodes=int(num_of_features * 0.08)
    output_dir="%s/Experiments/Tree/1" % (
        dataset_dir)

    command_to_run="python3 run_experiments3.py %s %s %s %d 0.5 %s" % (
        binary_similarities_filename, subsets_dirname, k_filename, number_of_nodes, output_dir)
    generate_run(command_to_run, run_filename)


if __name__ == "__main__":
    main()
