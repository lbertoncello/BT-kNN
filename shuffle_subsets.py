import sys
import os
import copy
from random import *
import shutil

dir_entrada = sys.argv[1]
dir_saida = sys.argv[2]
qtd = int(sys.argv[3])

conteudo_treino = []
conteudo_labels = []

with open(dir_entrada + '/treino.txt', 'r') as arquivo:
    conteudo_treino_original = arquivo.readlines()

with open(dir_entrada + '/classes.txt', 'r') as arquivo:
    conteudo_labels_original = arquivo.readlines()

script_execucao = '#!/bin/bash\n'

dir_raiz = dir_entrada.split('/')
dir_raiz = dir_raiz[0]

for j in range(qtd):
    conteudo_treino = copy.copy(conteudo_treino_original)
    conteudo_labels = copy.copy(conteudo_labels_original)
    novo_treino = []
    novo_labels = []

    dir_atual = "%s/%s/" % (dir_saida, j+1)
    dir_atual_adaptado = dir_atual + '/0'
    os.makedirs(dir_atual_adaptado, exist_ok=True)
    
    dir_experimento = '%s/Experiments/Random_Tree/%s' % (dir_raiz, j+1)
    os.makedirs(dir_experimento, exist_ok=True)

    i = len(conteudo_treino) - 1
    while i >= 0:
        index = randint(0, i)

        novo_treino.append(copy.copy(conteudo_treino[index]))
        #print('Antes: ' + str(len(conteudo_treino)))
        del conteudo_treino[index]
        #print('Depois: ' + str(len(conteudo_treino)))

        novo_labels.append(copy.copy(conteudo_labels[index]))
        del conteudo_labels[index]

        i -= 1

    script_execucao += 'python3 run_experiments3.py %s %s/k.txt 8 0.5 %s\n' % (dir_atual, dir_raiz, dir_experimento)

    with open(dir_atual_adaptado + '/treino.txt', 'w') as arquivo:
        arquivo.writelines(novo_treino)

    with open(dir_atual_adaptado + '/classes.txt', 'w') as arquivo:
        arquivo.writelines(novo_labels)

    shutil.copyfile(dir_entrada + '/entrada.txt', dir_atual_adaptado + '/entrada.txt')
    shutil.copyfile(dir_entrada + '/classes_corretas.txt', dir_atual_adaptado + '/classes_corretas.txt')

with open('./execucoes_R_%s_tree.sh' % (dir_raiz), 'w') as arquivo:
    arquivo.writelines(script_execucao)
    