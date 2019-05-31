#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Similarities
{
private:
    int mtx_first_position;
    vector<int> mtx_header;
    int mtx_line_size = sizeof(float);
    ifstream mtx_file;

    void read_header();
    int calc_position_at_mtx_file(int i, int j);

public:
    Similarities(const char *similarities_filename);
    Similarities() {};
    void init(const char *similarities_filename);
    double read_element_at(int i, int j);
    void close();
};