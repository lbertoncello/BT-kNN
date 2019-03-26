import numpy as np
import sys
import os
from sklearn.model_selection import StratifiedKFold

n_splits = 4

def create_subset(dataset_file_name, classes_file_name, subsets_dirname):
	dataset, classes = read_files(dataset_file_name, classes_file_name)

	np.set_printoptions(precision=2)

	skf = StratifiedKFold(n_splits=n_splits, shuffle=True)

	accuracy_avg = 0
	precision_avg = 0
	recall_avg = 0
	duration_avg = 0
	i = 0

	for train_index, test_index in skf.split(dataset, classes):
		dirname = "%s/%s" % (subsets_dirname, i)
		os.mkdir(dirname)

		with open('%s/treino.txt' % dirname, 'w') as f:
			for index in train_index:
				f.write("%s\n" % str(" ".join(map(str, dataset[index].astype(np.float)))))
			
		with open('%s/classes.txt' % dirname, 'w') as f:
			for index in train_index:
				f.write("%s\n" % str(" ".join(map(str, classes[index]))))
				
		with open('%s/entrada.txt' % dirname, 'w') as f:
			for index in test_index:
				f.write("%s\n" % str(" ".join(map(str, dataset[index].astype(np.float)))))
		
		with open('%s/classes_corretas.txt' % dirname, 'w') as f:
			for index in test_index:
				f.write("%s\n" % str(" ".join(map(str, classes[index]))))

		i += 1

def read_files(dataset_file_name, classes_file_name):
	dataset_file = open(dataset_file_name, 'r')
	lines = dataset_file.readlines()

	dataset_file.close()

	dataset = []

	for line in lines:
		vec = np.array(line.replace('\n', '').split(','))
		dataset.append(vec)
		#dataset.append(vec.astype(np.float))
		
	classes_file = open(classes_file_name, 'r')
	lines = classes_file.readlines()

	classes_file.close()

	classes = []

	for line in lines:
		vec = np.array(line.replace('\n', '').split(','))
		classes.append(vec)
	
	return (dataset, classes)

def main():
	dataset_file_name = sys.argv[1]
	classes_file_name = sys.argv[2]
	subsets_dirname = sys.argv[3]

	create_subset(dataset_file_name, classes_file_name, subsets_dirname)

if __name__ == "__main__":
    main()