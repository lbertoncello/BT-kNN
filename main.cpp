#include <iostream>
#include <string>
#include<vector>
#include<string>
#include <cstdlib>

#include "knn.h"

using namespace std;

int main(int argc, char* argv[])
{
	string similarities_file = argv[1];
	string train_file = argv[2];
	string classes_file = argv[3];
	string unclassified_documents_file = argv[4];
	string k_file = argv[5];
	int number_of_nodes = stoi(argv[6]);
	double decision_factor = stod(argv[7]);
	string output_dir = argv[8];
	kNN knn(number_of_nodes, decision_factor);

	cout << "Treinando" << endl;
	knn.train(similarities_file.c_str(), train_file.c_str(), classes_file.c_str());

	vector<int> k = read_k(k_file.c_str());

	cout << "Classificando" << endl;
	
	string mkdir = "mkdir -p ";
	string output_classification = "classifieds.txt";
	string dir;
	string barra = "/";
	for(int i = 0; i < k.size(); i++)
	{
		dir = output_dir + barra + to_string(i) + barra;
		system((mkdir + dir).c_str());
		if(argc > 9) {
			string output_probabilities = "prob.txt";

			knn.classify(k[i], unclassified_documents_file.c_str(), (dir + output_classification).c_str(), classes_file.c_str(), (dir + output_probabilities).c_str());
		} else {
			knn.classify(k[i], unclassified_documents_file.c_str(), (dir + output_classification).c_str());
		}
	}
	
	cout << "Classificacao realizada com sucesso!" << endl;

	knn.close();

	return 0;
}