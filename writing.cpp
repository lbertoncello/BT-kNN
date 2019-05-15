#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <iterator>
#include <iomanip>

#include "writing.h"

void write_results(vector<string> classes, const char *output_file)
{
	ofstream output;
	output_style(output);

	output.open(output_file);
	
	for (int i = 0; i < classes.size(); i++)
	{
		output << classes[i] << endl;
	}

	output.close();
}

void write_probabilities(vector<map<string, float>> posterior_probabilities, const char *output_file)
{
	ofstream output;
	output_style(output);

	output.open(output_file);

	for (int i = 0; i < posterior_probabilities.size(); i++)
	{
		for (map<string, float>::iterator it = posterior_probabilities[i].begin(); it != posterior_probabilities[i].end(); ++it)
		{
			output << it->first << " Prob.: " << it->second << " | ";
		}
		output << endl;
	}
}

void output_style(ofstream &saida)
{
	saida << std::fixed;
	saida << std::setprecision(6);
}

void print_times(double tempo_insercao, double tempo_rebalanceamento, double tempo_non_full, double tempo_metricas, double tempo_leitura, double tempo_calculo, int qtdLeituras, int qtdInsercoes, int qtd_comparacoes_insercao, bool houve_rebalanceamento)
{
	cout << "-------------------------------" << endl;
	cout << "Tempo Inserção: " << tempo_insercao << endl;
	cout << "Tempo Rebalanceamento: " << tempo_rebalanceamento << endl;
	cout << "Tempo Non Full: " << tempo_non_full << endl;
	cout << "Tempo Metricas: " << tempo_metricas << endl;
	cout << "Tempo Leitura: " << tempo_leitura << endl;
	cout << "Tempo Calculo: " << tempo_calculo << endl;
	cout << "Qtd Leituras: " << qtdLeituras << endl;
	cout << "Qtd Insercoes: " << qtdInsercoes << endl;
	cout << "Qtd Comparacoes Insercao: " << qtd_comparacoes_insercao << endl;
	cout << "Houve rebalanceamento: " << houve_rebalanceamento << endl;
	cout << "-------------------------------" << endl;
}

void write_times(string nome_arquivo, vector<double> tempo_insercao, vector<double> tempo_rebalanceamento, vector<double> tempo_non_full, vector<double> tempo_metricas, vector<double> tempo_leitura, vector<double> tempo_calculo, int qtdLeituras, int qtdInsercoes, int qtd_comparacoes_insercao, bool houve_rebalanceamento)
{
	ofstream saida;

	saida.open(nome_arquivo.c_str());

	for (int i = 0; i < tempo_insercao.size(); i++)
	{
		saida << "-------------------------------" << endl;
		saida << "Tempo Inserção: " << tempo_insercao[i] << endl;
		saida << "Tempo Rebalanceamento: " << tempo_rebalanceamento[i] << endl;
		saida << "Tempo Non Full: " << tempo_non_full[i] << endl;
		saida << "Tempo Metricas: " << tempo_metricas[i] << endl;
		saida << "Tempo Leitura: " << tempo_leitura[i] << endl;
		saida << "Tempo Calculo: " << tempo_calculo[i] << endl;
		saida << "Qtd Leituras: " << qtdLeituras << endl;
		saida << "Qtd Insercoes: " << qtdInsercoes << endl;
		saida << "Qtd Comparacoes Insercao: " << qtd_comparacoes_insercao << endl;
		saida << "Houve rebalanceamento: " << houve_rebalanceamento << endl;
		saida << "-------------------------------" << endl;
	}

	saida.close();
}