import sys
import struct

mtxFile = sys.argv[1]
binaryOutput = sys.argv[2]

with open(mtxFile, 'r') as f:
    mtx_line = f.readline()

    with open(binaryOutput, 'wb') as b:
        header = f.readline().replace('\n', '').split(' ')

        for info in header:
            b.write(struct.pack('@i', int(info)))

        mtx_line = f.readline()

        while mtx_line:
            data = mtx_line.replace('\n', '').split(' ')

            b.write(struct.pack('@f', float(data[2])))

            mtx_line = f.readline()

