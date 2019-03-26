from pathlib import Path
import sys

entrada = sys.argv[1]
saida = sys.argv[2]

matriz = list(map(
   lambda line: line.split(' '),
   Path(entrada).read_text().splitlines()[1:])
)
mtx = '%d %d %d\n' % (len(matriz), len(matriz[0]), len(matriz)*len(matriz[0]))
for line_num, line in enumerate(matriz):
   for col_num, col in enumerate(line):
       mtx += '%d %d %s\n' % (line_num+1, col_num+1, col)
Path(saida).write_text(mtx)