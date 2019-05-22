#include <vector>
#include <string>

#include "similarities.h"

using namespace std;

void Similarities::init(const char *similarities_filename)
{
    this->mtx_file.open(similarities_filename, std::ios::binary);
    read_header();
}

Similarities::Similarities(const char *similarities_filename)
{
    this->init(similarities_filename);
}

void Similarities::read_header()
{
    int number_of_rows;
    int number_of_columns;
    int number_of_non_nulls;
    vector<int> header;

    mtx_file.read((char *)&number_of_rows, sizeof(int));
    mtx_file.read((char *)&number_of_columns, sizeof(int));
    mtx_file.read((char *)&number_of_non_nulls, sizeof(int));

    header.push_back(number_of_rows);
    header.push_back(number_of_columns);
    header.push_back(number_of_non_nulls);

    mtx_first_position = mtx_file.tellg();
    mtx_header = header;
}

int Similarities::calc_position_at_mtx_file(int i, int j)
{
    return mtx_first_position + ((i * mtx_header[1]) * mtx_line_size) + (j * mtx_line_size);
}

double Similarities::read_element_at(int i, int j)
{
    double value;

    int position_at_file = calc_position_at_mtx_file(i, j);

    mtx_file.seekg(position_at_file);
    mtx_file.read((char *)&value, sizeof(double));

    return value;
}

void Similarities::close()
{
    this->mtx_file.close();
}