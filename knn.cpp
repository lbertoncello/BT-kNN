#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iterator>

#include "knn.h"
#include "writing.h"

using namespace std;

map<string, int> current_counter;

kNN::kNN(int number_of_nodes, double decision_factor)
{
	model = BTree(number_of_nodes, decision_factor);
}

bool comparator(Document *d1, Document *d2)
{
	return d1->similarity < d2->similarity;
}

void kNN::train(const char *similarities_file, const char *train_file, const char *classes_file)
{
	this->train_file.open(train_file, std::ifstream::in);
	this->sims.init(similarities_file);
	this->model.generate_tree(this->train_file, this->sims, classes_file);
}

string most_frequent_class(vector<Document *> nearest_neighbors, int k)
{
	map<string, int> counter;
	int maior = 0;
	string chosen_class;

	if (k > nearest_neighbors.size())
	{
		k = nearest_neighbors.size();
	}

	for (int i = 0; i < k; i++)
	{
		//Verifica se o elemento j� est� no map
		if (counter.find(nearest_neighbors[i]->doc_class) != counter.end())
		{
			counter[nearest_neighbors[i]->doc_class]++;
		}
		else
		{
			counter[nearest_neighbors[i]->doc_class] = 1;
		}
	}

	current_counter = counter;

	for (std::map<string, int>::iterator it = counter.begin(); it != counter.end(); ++it)
	{
		if (it->second > maior)
		{
			maior = it->second;
			chosen_class = it->first;
		}
	}

	return chosen_class;
}

float posterior_probability(float km, float k)
{
	return km / k;
}

map<string, float> posterior_probabilities(vector<Document *> nearest_neighbors, int k, map<string, float> distinct_classes)
{
	map<string, int> classes_counter;
	map<string, float> classes_probabilities = distinct_classes;
	int original_k = k;

	if (k > nearest_neighbors.size())
	{
		k = nearest_neighbors.size();
	}

	for (int i = 0; i < k; i++)
	{
		//Verifica se o elemento j� est� no map
		if (classes_counter.find(nearest_neighbors[i]->doc_class) != classes_counter.end())
		{
			classes_counter[nearest_neighbors[i]->doc_class]++;
		}
		else
		{
			classes_counter[nearest_neighbors[i]->doc_class] = 1;
		}
	}

	for (map<string, int>::iterator it = classes_counter.begin(); it != classes_counter.end(); ++it)
	{
		classes_probabilities.find(it->first)->second = posterior_probability(it->second, original_k);
	}

	return classes_probabilities;
}

map<string, float> posterior_probabilities(vector<Document *> nearest_neighbors, int k, map<string, float> distinct_classes, map<string, int> classes_counter)
{
	map<string, float> classes_probabilities = distinct_classes;

	if (k > nearest_neighbors.size())
	{
		k = nearest_neighbors.size();
	}

	for (map<string, int>::iterator it = classes_counter.begin(); it != classes_counter.end(); ++it)
	{
		classes_probabilities.find(it->first)->second = posterior_probability((float)it->second, (float)k);
	}

	return classes_probabilities;
}

//Calcula as probabilidades para todas as entradas
void kNN::probabilities(int k, const char *unclassified_documents_file, const char *classes_file, const char *prob_output_file)
{
	vector<string> classes;
	vector<int> documents_id = read_unclassified_documents(unclassified_documents_file);
	map<string, float> distinct_classes = read_distinct_classes(classes_file);
	vector<map<string, float>> classes_probabilities;

	for (int i = 0; i < documents_id.size(); i++)
	{
		vector<Document *> nearest_neighbors;

		this->train_file.clear();

		model.most_similar_documents(this->sims, documents_id[i], k, nearest_neighbors);

		//nearest_neighbors.pop_back();
		std::sort(nearest_neighbors.rbegin(), nearest_neighbors.rend(), comparator);
		classes_probabilities.push_back(posterior_probabilities(nearest_neighbors, k, distinct_classes));
	}

	write_probabilities(classes_probabilities, prob_output_file);
}

void kNN::classify(int k, const char *unclassified_documents_file, const char *results_output_file)
{
	vector<string> classes;
	vector<int> documents_id = read_unclassified_documents(unclassified_documents_file);

	for (int i = 0; i < documents_id.size(); i++)
	{
		vector<Document *> nearest_neighbors;

		this->train_file.clear();

		model.most_similar_documents(this->sims, documents_id[i], k, nearest_neighbors);

		if (nearest_neighbors.size() > 1)
		{
			//nearest_neighbors.pop_back();
			std::sort(nearest_neighbors.rbegin(), nearest_neighbors.rend(), comparator);
			classes.push_back(most_frequent_class(nearest_neighbors, k));
		}
		else
		{
			classes.push_back(nearest_neighbors[0]->doc_class);
		}
	}

	write_results(classes, results_output_file);
}

void kNN::classify(int k, const char *unclassified_documents_file, const char *results_output_file, const char *classes_file, const char *prob_output_file)
{
	vector<string> classes;
	vector<int> documents_id = read_unclassified_documents(unclassified_documents_file);
	map<string, float> distinct_classes = read_distinct_classes(classes_file);
	vector<map<string, float>> classes_probabilities;

	for (int i = 0; i < documents_id.size(); i++)
	{
		vector<Document *> nearest_neighbors;

		this->train_file.clear();

		model.most_similar_documents(this->sims, documents_id[i], k, nearest_neighbors);

		if (nearest_neighbors.size() > 1)
		{
			//nearest_neighbors.pop_back();
			std::sort(nearest_neighbors.rbegin(), nearest_neighbors.rend(), comparator);
			classes.push_back(most_frequent_class(nearest_neighbors, k));
		}
		else
		{
			classes.push_back(nearest_neighbors[0]->doc_class);
		}
		classes_probabilities.push_back(posterior_probabilities(nearest_neighbors, k, distinct_classes, current_counter));
	}

	write_results(classes, results_output_file);
	write_probabilities(classes_probabilities, prob_output_file);
}

void kNN::close()
{
	this->train_file.close();
}
