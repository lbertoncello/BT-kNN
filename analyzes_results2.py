from sklearn.metrics import recall_score
from sklearn.metrics import precision_score
from sklearn.metrics import accuracy_score


def read_results(correct, predicted):
	file = open(correct, 'r')
	correct_classes = file.readlines()
	file.close()

	file = open(predicted, 'r')
	predicted_classes = file.readlines()
	file.close()

	return correct_classes, predicted_classes

def generate_confusion_matrix(correct_classes, predicted_classes):
	confusion_matrix = {}

	#Inicializing the confusion matrix
	for cl in correct_classes:
		if (cl in confusion_matrix) == False:
			confusion_matrix[cl] = {}

	for cl in confusion_matrix:
		for _cl in confusion_matrix:
			confusion_matrix[cl][_cl] = 0

	for i in range(len(correct_classes)):
		confusion_matrix[correct_classes[i]][predicted_classes[i]] += 1
		
	return confusion_matrix

def metrics_calc(confusion_matrix):
	true_positive = {}
	true_negative = {}
	false_positive = {}
	false_negative = {}

	accuracy = {}
	precision = {}
	recall = {}
	accuracy_avg = 0
	precision_avg = 0
	recall_avg = 0

	#Inicializa os valores
	for cl in confusion_matrix:
		true_negative[cl] = 0
		false_negative[cl] = 0
		false_positive[cl] = 0

	for cl in confusion_matrix:
		true_positive[cl] = confusion_matrix[cl][cl]

		for _cl in confusion_matrix:
			if _cl != cl:
				false_positive[cl] += confusion_matrix[_cl][cl]

		for _cl in confusion_matrix:
			if _cl != cl:
				false_negative[cl] += confusion_matrix[cl][_cl]

		for _cl in confusion_matrix:
			if _cl != cl:
				for __cl in confusion_matrix:
					if __cl != cl:
						true_negative[cl] += confusion_matrix[_cl][__cl] 

		if true_positive[cl] != 0:
			accuracy[cl] = (true_positive[cl] + true_negative[cl]) / (true_positive[cl] + true_negative[cl] + false_positive[cl] + false_negative[cl])
			precision[cl] = true_positive[cl] / (true_positive[cl] + false_positive[cl])
			recall[cl] = true_positive[cl] / (true_positive[cl] + false_negative[cl])
		else:
			accuracy[cl] = 0
			precision[cl] = 0
			recall[cl] = 0

		accuracy_avg += accuracy[cl]
		precision_avg += precision[cl]
		recall_avg += recall[cl]

	accuracy_avg = accuracy_avg / len(accuracy)
	precision_avg = precision_avg / len(precision)
	recall_avg = recall_avg / len(recall)
	
	#print("Accuracy: %s" % accuracy_avg)
	#print("Precision: %s" % precision_avg)
	#print("Recall: %s" % recall_avg)

	return accuracy_avg, precision_avg, recall_avg

def analyze(correct, predicted):
	correct_classes, predicted_classes = read_results(correct, predicted)

	accuracy = accuracy_score(correct_classes, predicted_classes)

	precision_micro = precision_score(correct_classes, predicted_classes, average='micro')
	precision_macro = precision_score(correct_classes, predicted_classes, average='macro')

	recall_micro = recall_score(correct_classes, predicted_classes, average='micro')
	recall_macro = recall_score(correct_classes, predicted_classes, average='macro')

	return accuracy, precision_micro, precision_macro, recall_micro, recall_macro
