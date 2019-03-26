import numpy as np
from sklearn import neighbors, datasets, model_selection
from sklearn.metrics.pairwise import cosine_similarity

def write_results(results, dataset_dirname):
    with open('%s/classifieds.txt' % dataset_dirname, 'w') as f:
        for result in results:
            f.write("%s\n" % str("".join(map(str, result))))

def cosine(a,b):
    return 1 - cosine_similarity(a.reshape(1, -1), b.reshape(1, -1))[0][0]

def classify(k, train_dataset, train_classes, test_dataset, dataset_dirname):
    knn = neighbors.KNeighborsClassifier(n_neighbors=k, algorithm='brute', metric=lambda a,b: cosine(a,b))
    #knn = neighbors.KNeighborsClassifier(n_neighbors=k, algorithm='brute')

    

    knn.fit(train_dataset, train_classes)
    results = knn.predict(test_dataset)

    write_results(results, dataset_dirname)

'''

iris = datasets.load_iris()

#knn = neighbors.NearestNeighbors(n_neighbors=3, algorithm="brute")

skf = model_selection.StratifiedKFold(n_splits=4, shuffle=True)

dataset = iris.data
classes = iris.target

for train_index, test_index in skf.split(dataset, classes):
    train_dataset = np.zeros([len(train_index), len(dataset[0])])
    train_classes = np.zeros(len(train_index))
    test_dataset = np.zeros([len(test_index), len(dataset[0])])
    test_classes = np.zeros(len(test_index))

    for i in range(len(train_index)):
        train_dataset[i] = dataset[train_index[i]]
			
    for i in range(len(train_index)):
        train_classes[i] = classes[train_index[i]]
				
    for i in range(len(test_index)):
        test_dataset[i] = dataset[test_index[i]]
		
    for i in range(len(test_index)):
        test_classes[i] = classes[test_index[i]]

    
'''