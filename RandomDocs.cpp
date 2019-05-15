#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm> 
#include <sstream> 
#include <iterator> 

using namespace std;

#define NUM_OF_LINES 5
#define NUM_OF_COLUMNS 4
#define FILE_NAME "docs.txt"

void write_vector(const char* file_name, vector<vector<float>> docs)
{
	ofstream saida;
	vector<ostringstream> docs_s;

	if (!docs.empty())
	{
		ostringstream vec;

		for (int i = 0; i < docs.size(); i++)
		{
			// Convert all but the last element to avoid a trailing "," 
			std::copy(docs[i].begin(), docs[i].end() - 1,
				std::ostream_iterator<float>(vec, " "));

			// Now add the last element with no delimiter 
			vec << docs[i].back();
		}

		docs_s.push_back(vec);
	}

	saida.open(file_name);

	for (int i = 0; i < docs_s.size(); i++)
	{
		saida << docs_s[i].str() << endl;
	}

	saida.close();
}

int main()
{
	vector<vector<float>> docs;

	for (int i = 0; i < NUM_OF_LINES; i++)
	{
		vector<float> vec;

		for (int j = 0; j < NUM_OF_COLUMNS; j++)
		{
			vec.push_back(rand());
		}

		docs.push_back(vec);
	}

	write_vector(FILE_NAME, docs);

	return 0;
}