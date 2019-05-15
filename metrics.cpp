#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

#include "metrics.h"

using namespace std;

double norm(vector<double> a)
{
	double sum = 0;

	for (vector<double>::iterator it = a.begin(); it != a.end(); ++it)
	{
		sum += (*it * *it);
	}

	return sqrt(sum);
}

double cosine_similarity(vector<double> a, vector<double> b)
{
	if(norm(a) == 0 || norm(b) == 0) {
		return 0;
	} 
	return inner_product(begin(a), end(a), begin(b), 0.0) / (norm(a) * norm(b));
}