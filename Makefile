

TCGA2015: TCGA2015.o NonogramSolver.o
	g++-6 -fopenmp -std=c++11 -o ../speedtest/TCGA2015v1-4.out TCGA2015.o NonogramSolver.o

TCGA2015.o: TCGA2015.cpp NonogramSolver.h
	g++-6 -fopenmp -std=c++11 -c TCGA2015.cpp 
	
NonogramSolver.o: NonogramSolver.h NonogramSolver.cpp NonogramSolver.tpp Line.tpp
	g++-6 -fopenmp -std=c++11 -c NonogramSolver.cpp

clean:
	rm -f *.o