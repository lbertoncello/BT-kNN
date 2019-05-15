import numpy as np
from sklearn.model_selection import StratifiedKFold

def create_subset(dataset_file_name, classes_file_name):
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
