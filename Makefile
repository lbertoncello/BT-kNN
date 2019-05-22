all:
	g++ -std=c++11 -Ofast metrics.cpp reading.cpp similarities.cpp b-tree.cpp knn.cpp writing.cpp main.cpp -o knn

clean:
	rm *.o