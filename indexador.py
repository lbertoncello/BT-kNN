import sys
import os
import numpy as np
from sklearn.feature_extraction.text import TfidfVectorizer

def indexa(conteudo_arquivos, nome_saida, lista_tokens):
    vectorizer = TfidfVectorizer(vocabulary=lista_tokens, use_idf=True, lowercase=True)
    #print(vectorizer.get_feature_names())
    #print('qtd: ' + str(len(vectorizer.get_feature_names())))
    vetores_arquivos = vectorizer.fit_transform(conteudo_arquivos)

    escreve_vetores(vetores_arquivos, nome_saida)

def escreve_vetores(vetores, nome_saida):
    with open(nome_saida, 'w') as f:
        for vetor in vetores:
            f.write('%s\n' % np.array2string(vetor, precision=2, separator=',', max_line_width=999999).lstrip('[').rstrip(']'))
    

def leArquivos(nome_lista_arquivos):
    dirname = os.path.dirname(nome_lista_arquivos)

    lista_arquivos = []

    with open(nome_lista_arquivos, 'r') as f:
        lista_arquivos = f.readlines()

    conteudo_arquivos = []

    for nome_arquivo in lista_arquivos:
        nome_arquivo = dirname + '/' + nome_arquivo.replace('\n', '')

        with open(nome_arquivo, 'r', encoding="latin-1") as f:
            conteudo_arquivos.append(''.join(f.readlines()))

    return conteudo_arquivos

def leTokens(nome_lista_tokens):
    tokens = []

    with open(nome_lista_tokens, 'r') as f:
        tokens = f.readlines()

    for i in range(len(tokens)):
        tokens[i] = tokens[i].replace('\n', '')

    return tokens



def main():
    nome_lista_arquivos = sys.argv[1]
    nome_saida = sys.argv[2]

    if len(sys.argv) > 3:
        lista_tokens = leTokens(sys.argv[3])
    else:
        lista_tokens = []
    
    conteudo_arquivos = leArquivos(nome_lista_arquivos)    
    indexa(conteudo_arquivos, nome_saida, lista_tokens)

if __name__ == "__main__":
    main()