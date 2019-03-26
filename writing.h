#include <vector>
#include <string>

using namespace std;

void write_results(vector<string> classes, const char* output_file);

void print_times(double tempo_insercao, double tempo_rebalanceamento, double tempo_non_full, double tempo_metricas, double tempo_leitura, double tempo_calculo, int qtdLeituras, int qtdInsercoes, int qtd_comparacoes_insercao, bool houve_rebalanceamento);

void write_times(string nome_arquivo, vector<double> tempo_insercao, vector<double> tempo_rebalanceamento, vector<double> tempo_non_full, vector<double> tempo_metricas, vector<double> tempo_leitura, vector<double> tempo_calculo, int qtdLeituras, int qtdInsercoes, int qtd_comparacoes_insercao, bool houve_rebalanceamento);