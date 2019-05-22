import sys
import struct

mtxFile = sys.argv[1]
binaryOutput = sys.argv[2]

with open(mtxFile, 'r') as f:
    mtx_lines = f.readlines()

mtx_lines = mtx_lines[1:]

with open(binaryOutput, 'wb') as f:
    header = mtx_lines[0].replace('\n', '').split(' ')

    for info in header:
        f.write(struct.pack('@i', int(info)))

    mtx_lines = mtx_lines[1:]

    for line in mtx_lines:
        data = line.replace('\n', '').split(' ')
        
        f.write(struct.pack('@d', float(data[2])))