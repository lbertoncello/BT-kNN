import sys
import os
import numpy as np
import time
import analyzes_results_A as ar

n_splits = 1
		
def run_tests(dirname, k_list, k_file, number_of_nodes, decision_factor, output_dir):
	durations = []

	for i in range(n_splits):
		print("-----------------------Classifying-----------------------")
		dataset_dirname = "%s/%s" % (dirname, i)

		start = time.time()
		os.system("./knn %s/treino.txt %s/classes.txt %s/entrada.txt %s %s %s %s" % (dataset_dirname, dataset_dirname, 
			dataset_dirname, k_file, number_of_nodes, decision_factor, output_dir + "/" + str(i) ))
		end = time.time()

		durations.append(end - start)

	for i in range(len(k_list)):
		accuracy_avg = 0
		precision_micro_avg = 0
		precision_macro_avg = 0
		recall_micro_avg = 0
		recall_macro_avg = 0
		f1_micro_avg = 0
		f1_macro_avg = 0
		duration_avg = 0

		print("--------------------Analising Results--------------------")
		for j in range(n_splits):
			dataset_dirname = "%s/%s" % (dirname, j)

			accuracy, precision_micro, recall_micro = ar.analyze(dataset_dirname + "/classes_corretas.txt", output_dir + "/" + str(0) + "/" + str(i) + "/classifieds.txt")

			accuracy_avg += accuracy
			precision_micro_avg += precision_micro
			recall_micro_avg += recall_micro
		
		accuracy_avg = accuracy_avg / n_splits
		precision_micro_avg = precision_micro_avg / n_splits
		recall_micro_avg = recall_micro_avg / n_splits

		print_results(accuracy_avg, precision_micro_avg, recall_micro_avg)
		write_results(output_dir + "/" + str(0), accuracy_avg, precision_micro_avg, recall_micro_avg, i, k_list[i], number_of_nodes, decision_factor)


def print_results(accuracy_avg, precision_micro, recall_micro):
	print("=======================Result=======================")
	print("Average Accuracy: %s" % accuracy_avg)
	print("Precision Micro: %s" % precision_micro)
	print("Recall Micro: %s" % recall_micro)
	print("====================================================")

def write_results(dir, accuracy, precision_micro, recall_micro, i, k, number_of_nodes, decision_factor):
	os.makedirs(dir + "/" + str(i), exist_ok=True)
	
	with open('%s/results.txt' % (dir + "/" + str(i)), 'w') as f:
		f.write("%s\n" % accuracy)
		f.write("%s\n" % precision_micro)
		f.write("%s\n" % recall_micro)

	with open('%s/experiment_data.txt' % (dir + "/" + str(i)), 'w') as f:
		f.write("%s\n" % k)
		f.write("%s\n" % number_of_nodes)
		f.write("%s\n" % decision_factor)

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
	
	k_list = read_k(k_file)
			
	run_tests(dirname, k_list, k_file, number_of_nodes, decision_factor, output_dir)	


if __name__ == "__main__":
    main()
