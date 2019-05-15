import sys
import os

def remove_empty_lines(features_filename, labels_filename, output_dirname):
    with open(features_filename, 'r') as f:
        features = f.readlines()

    with open(labels_filename, 'r') as f:
        labels = f.readlines()

    i = 0
    while i < len(features):
        if features[i] == '\n':
            del features[i]
            del labels[i]
        else:
            i += 1

    with open(output_dirname + 'features_v2.csv', 'w') as f:
        f.writelines(features)

    with open(output_dirname + '/labels_v2.txt', 'w') as f:
        f.writelines(labels)


def main():
    features_filename = sys.argv[1]
    labels_filename = sys.argv[2]
    output_dirname = sys.argv[3]
    
    remove_empty_lines(features_filename, labels_filename, output_dirname)

if __name__ == "__main__":
    main()