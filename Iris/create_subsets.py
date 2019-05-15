import sys
import numpy as np
from sklearn.model_selection import train_test_split

dataset_file = sys.argv[1]
test_size = float(sys.argv[2])

file = open(dataset_file, 'r')
lines = file.readlines()

file.close()

dataset = []

for line in lines:
	vec = np.array(line.replace('\n', '').split(','))
	dataset.append(vec)
	#dataset.append(vec.astype(np.float))
	
X_train, X_test = train_test_split(dataset, test_size=test_size)

np.set_printoptions(precision=2)

with open('treino.txt', 'w') as f:
    for vec in X_train:
        f.write("%s\n" % str(" ".join(map(str, vec[:4].astype(np.float)))))

with open('classes.txt', 'w') as f:
    for vec in X_train:
        f.write("%s\n" % str(vec[4]))
		
with open('entrada.txt', 'w') as f:
    for vec in X_test:
        f.write("%s\n" % str(" ".join(map(str, vec[:4].astype(np.float)))))
		
with open('classes_corretas.txt', 'w') as f:
    for vec in X_test:
        f.write("%s\n" % str(vec[4]))
	
