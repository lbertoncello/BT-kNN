import sys
import best_run as br

def find_best_average(dirname, k_file, number_of_nodes, decision_factor, number_of_tries, output_dir, n_initial, n_final):
    average_accuracy = 0

    for i in range(n_initial, n_final + 1):
        exp_output_dir = "%s/%s" % (output_dir, str(i))
        accuracy, experiment = br.find_best(dirname, k_file, number_of_nodes, decision_factor, number_of_tries, exp_output_dir)
        average_accuracy += accuracy

    average_accuracy = average_accuracy / (n_final - n_initial + 1)
    return average_accuracy

def write_average_accuracy(accuracy, number_of_tries, filename):
    with open(filename, 'w') as f:
        f.write('%s\n%s' % (str(accuracy), str(number_of_tries)))

def main():
    dirname = sys.argv[1]
    k_file = sys.argv[2]
    number_of_nodes = sys.argv[3]
    decision_factor = sys.argv[4]
    output_dir = sys.argv[5]
    number_of_tries = int(sys.argv[6])
    n_initial = int(sys.argv[7])
    n_final = int(sys.argv[8])

    accuracy = find_best_average(dirname, k_file, number_of_nodes, decision_factor, number_of_tries, output_dir, n_initial, n_final)
    print('Average accuracy: ' + str(accuracy))

    filename = '%s/average-%s-%s.txt' % (output_dir, str(n_initial), str(n_final))
    write_average_accuracy(accuracy, number_of_tries, filename)
    

if __name__ == "__main__":
    main()

