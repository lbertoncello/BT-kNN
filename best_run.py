import sys
import os
import numpy as np

def print_test():
    print('testando')

def find_best(dirname, k_file, number_of_nodes, decision_factor, number_of_tries, output_dir):
    experiment_id = str(np.random.randint(100000000))
    j = 0
    number_of_k = len(read_k(k_file))
    current_dirname = dirname
    best_accuracy = 0
    best_experiment = ''

    i = 0
    while i < number_of_tries:
        temp_experiment_dir = './temp-%s/Experiments/%s/' % (experiment_id, j)
        
        os.system('python3 run_experiments3.py %s %s %s %s %s' % (current_dirname, k_file, number_of_nodes, decision_factor, temp_experiment_dir))

        for n in range(number_of_k):
            results_filename = '%s/0/%s/results.txt' % (temp_experiment_dir, str(n))
            accuracy = read_accuracy(results_filename)

            if accuracy > best_accuracy:
                best_accuracy = accuracy
                best_experiment = '%s/0/%s' % (temp_experiment_dir, str(n))
                i = 0

        temp_subset_dir = './temp-%s/Rnd_Subsets/%s/' % (experiment_id, j)
        os.system('python3 shuffle_subsets.py %s %s 1' % (dirname + '/0/', temp_subset_dir))
        current_dirname = temp_subset_dir + '/1/'

        i += 1
        j += 1

    print('Numero total de tentativas: ' + str(j))

    os.makedirs(output_dir, exist_ok=True)
    os.system('mv %s %s' % (best_experiment, output_dir))
    os.system('rm -r ./temp-%s' % (experiment_id))

    return best_accuracy, best_experiment

def read_accuracy(filename):
    with open(filename, 'r') as f:
        return float(f.readline())


def read_k(k_file):
    file = open(k_file, 'r')
    text = file.readlines()
    file.close()

    k = []

    for line in text:
        k.append(int(line))

    return k

def main():
    dirname = sys.argv[1]
    k_file = sys.argv[2]
    number_of_nodes = sys.argv[3]
    decision_factor = sys.argv[4]
    output_dir = sys.argv[5]
    number_of_tries = int(sys.argv[6])

    accuracy, experiment = find_best(dirname, k_file, number_of_nodes, decision_factor, number_of_tries, output_dir)

    print('Best accuracy: ' + str(accuracy))
    print('Best experiment: ' + experiment)

if __name__ == "__main__":
    main()