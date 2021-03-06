import sys
import os
import numpy as np
import time
from sklearn.model_selection import StratifiedKFold
import create_subsets_sklearn as cs
import analyzes_results_A as ar
import sklearn_knn as sk_knn

n_splits = 4
		
def run_tests(dirname, k, output_dir):
	accuracy_avg = 0
	precision_micro_avg = 0
	recall_micro_avg = 0

	for i in range(n_splits):
		dataset_dirname = "%s/%s" % (dirname, i)

		file = open(dataset_dirname + "/treino.txt", "r")
		text = file.readlines()
		file.close()

		train_dataset = np.zeros([len(text), text[0].count(' ') + 1])

		i = 0
		for line in text:
			j = 0
			vec = line.split(' ')

			for num in vec:
				train_dataset[i][j] = float(num)
				j += 1
			i += 1

		file = open(dataset_dirname + "/classes.txt", "r")
		text = file.readlines()
		file.close()

		train_classes = np.chararray(len(text), itemsize=30, unicode=True)

		i = 0
		for line in text:
			train_classes[i] = line.replace('\n', '')
			i += 1

		file = open(dataset_dirname + "/entrada.txt", "r")
		text = file.readlines()
		file.close()

		test_dataset = np.zeros([len(text), text[0].count(' ') + 1])

		i = 0
		for line in text:
			j = 0
			vec = line.split(' ')

			for num in vec:
				test_dataset[i][j] = float(num)
				j += 1
			i += 1

		start = time.time()
		sk_knn.classify(k, train_dataset, train_classes, test_dataset, dataset_dirname)
		end = time.time()

		duration = end - start	

		accuracy, precision_micro, recall_micro = ar.analyze(dataset_dirname + "/classes_corretas.txt", dataset_dirname + "/classifieds.txt")

		accuracy_avg += accuracy
		precision_micro_avg += precision_micro
		recall_micro_avg += recall_micro
	
	accuracy_avg = accuracy_avg / n_splits
	precision_micro_avg = precision_micro_avg / n_splits
	recall_micro_avg = recall_micro_avg / n_splits

	print_results(accuracy, precision_micro_avg, recall_micro_avg)
	write_results(output_dir, accuracy, precision_micro_avg, recall_micro_avg, k)

def print_results(accuracy_avg, precision_micro, recall_micro):
	print("=======================Result=======================")
	print("Average Accuracy: %s" % accuracy_avg)
	print("Precision Micro: %s" % precision_micro)
	print("Recall Micro: %s" % recall_micro)
	print("====================================================")

def write_results(dir, accuracy, precision_micro, recall_micro, k):
	os.makedirs(dir + "/", exist_ok=True)
	
	with open('%s/results.txt' % (dir + "/"), 'w') as f:
		f.write("%s\n" % accuracy)
		f.write("%s\n" % precision_micro)
		f.write("%s\n" % recall_micro)

	with open('%s/experiment_data.txt' % (dir + "/"), 'w') as f:
		f.write("%s\n" % k)

def main():
	dirname = sys.argv[1]
	k = int(sys.argv[2])
	output_dir = sys.argv[3]

	run_tests(dirname, k, output_dir)

if __name__ == "__main__":
    main()