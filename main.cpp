#include <iostream>
#include <string>
#include<vector>
#include<string>
#include <cstdlib>

#include "knn.h"

using namespace std;

int main(int argc, char* argv[])
{
	string train_file = argv[1];
	string classes_file = argv[2];
	string unclassified_documents_file = argv[3];
	string k_file = argv[4];
	int number_of_nodes = stoi(argv[5]);
	double decision_factor = stod(argv[6]);
	string output_dir = argv[7];
	kNN knn(number_of_nodes, decision_factor);

	cout << "Treinando" << endl;
	knn.train(train_file.c_str(), classes_file.c_str());

	vector<int> k = read_k(k_file.c_str());

	cout << "Classificando" << endl;
	
	string mkdir = "mkdir -p ";
	string arquivo = "classifieds.txt";
	string dir;
	string barra = "/";
	for(int i = 0; i < k.size(); i++)
	{
		dir = output_dir + barra + to_string(i) + barra;
		system((mkdir + dir).c_str());
		knn.classify(k[i], unclassified_documents_file.c_str(), (dir + arquivo).c_str());
	}
	
	cout << "Classificacao realizada com sucesso!" << endl;

	knn.close();

	return 0;
}