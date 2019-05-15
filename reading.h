#include <vector>
#include <map>

using namespace std;

int number_of_columns(const char* train_file);
int number_of_lines(const char* train_file);
vector<double> read_vector(string line);
vector<double> read_at_index(ifstream& train_file, int pos);
string read_class_at_index(const char* classes_file, int line_number);
vector<string> read_classes(const char* classes_file);
map<string, float> read_distinct_classes(const char* classes_file);
vector<vector<double>> read_unclassified_documents(const char* documents_file);
vector<int> read_k(const char* k_file);