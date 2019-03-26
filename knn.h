#include <fstream>
#include <iostream>

#include "b-tree.h"

using namespace std;

class kNN
{
private:
	BTree model;
	ifstream train_file;

public:
	kNN(int number_of_nodes, double decision_factor);
	void train(const char* train_file, const char* classes_file);
	void classify(int k, const char* unclassified_documents_file, const char* output_file);
	void close();
};