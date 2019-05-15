import sys
import os
import numpy as np
import time
from sklearn.model_selection import StratifiedKFold
import create_subsets as cs
import analyzes_results as ar

n_splits = 4
		
def run_tests(dataset_file_name, classes_file_name, k, number_of_nodes, decision_factor):
	dataset, classes = cs.create_subset(dataset_file_name, classes_file_name)
	dataset_dirname = os.path.dirname(dataset_file_name)
	np.set_printoptions(precision=2)

	skf = StratifiedKFold(n_splits=n_splits, shuffle=True)

	accuracy_avg = 0
	precision_avg = 0
	recall_avg = 0
	duration_avg = 0

	for train_index, test_index in skf.split(dataset, classes):
		with open('%s/treino.txt' % dataset_dirname, 'w') as f:
			for index in train_index:
				f.write("%s\n" % str(" ".join(map(str, dataset[index].astype(np.float)))))
			
		with open('%s/classes.txt' % dataset_dirname, 'w') as f:
			for index in train_index:
				f.write("%s\n" % str(" ".join(map(str, classes[index]))))
				
		with open('%s/entrada.txt' % dataset_dirname, 'w') as f:
			for index in test_index:
				f.write("%s\n" % str(" ".join(map(str, dataset[index].astype(np.float)))))
		
		with open('%s/classes_corretas.txt' % dataset_dirname, 'w') as f:
			for index in test_index:
				f.write("%s\n" % str(" ".join(map(str, classes[index]))))
		
		start = time.time()
		os.system("./knn %s/treino.txt %s/classes.txt %s/entrada.txt %s %s %s %s/classifieds.txt > /dev/null" % (dataset_dirname, dataset_dirname, 
			dataset_dirname, k, number_of_nodes, decision_factor, dataset_dirname))
		end = time.time()

		duration = end - start	

		accuracy, precision, recall = ar.analyze(dataset_dirname + "/classes_corretas.txt", dataset_dirname + "/classifieds.txt")

		accuracy_avg += accuracy
		precision_avg += precision
		recall_avg += recall
		duration_avg += duration
	
	accuracy_avg = accuracy_avg / n_splits
	precision_avg = precision_avg / n_splits
	recall_avg = recall_avg / n_splits
	duration_avg = duration_avg / n_splits

	return accuracy_avg, precision_avg, recall_avg, duration_avg

def print_results(accuracy_avg, precision_avg, recall_avg, duration_avg):
	print("=======================Result=======================")
	print("Average Accuracy: %s" % accuracy_avg)
	print("Average Precision: %s" % precision_avg)
	print("Average Recall: %s" % recall_avg)
	print("Time: %s" % duration_avg)
	print("====================================================")

def write_results(dir, accuracy_avg, precision_avg, recall_avg, duration_avg, k, number_of_nodes, decision_factor, number_of_tests):
	with open('%s/results.txt' % dir, 'w') as f:
		f.write("%s\n" % accuracy_avg)
		f.write("%s\n" % precision_avg)
		f.write("%s\n" % recall_avg)
		f.write("%s\n" % duration_avg)

	with open('%s/experiment_data.txt' % dir, 'w') as f:
		f.write("%s\n" % k)
		f.write("%s\n" % number_of_nodes)
		f.write("%s\n" % decision_factor)
		f.write("%s\n" % number_of_tests)

def main():
	dataset_file_name = sys.argv[1]
	classes_file_name = sys.argv[2]
	k = int(sys.argv[3])
	number_of_nodes = sys.argv[4]
	decision_factor = sys.argv[5]
	number_of_tests = int(sys.argv[6])
	output_dir = sys.argv[7]

	accuracy_avg = 0
	precision_avg = 0
	recall_avg = 0
	duration_avg = 0

	print("---------------------Experiment---------------------")

	for i in range(number_of_tests):

		accuracy, precision, recall, duration = run_tests(dataset_file_name, classes_file_name, k, number_of_nodes, decision_factor)

		print("#################Test#################")
		print("Accuracy: %s" % accuracy)
		print("Precision: %s" % precision)
		print("Recall: %s" % recall)
		print("Time: %s" % duration)
		print("######################################")
		
		accuracy_avg += accuracy
		precision_avg += precision
		recall_avg += recall
		duration_avg += duration

	print("----------------------------------------------------")

	accuracy_avg = accuracy_avg / number_of_tests
	precision_avg = precision_avg / number_of_tests
	recall_avg = recall_avg / number_of_tests
	duration_avg = duration_avg / number_of_tests

	print_results(accuracy_avg, precision_avg, recall_avg, duration_avg)
	write_results(output_dir, accuracy_avg, precision_avg, recall_avg, duration_avg, k, number_of_nodes, decision_factor, number_of_tests)
	

if __name__ == "__main__":
    main()