

TCGA2015: TCGA2015.o NonogramSolver.o
	g++-6 -fopenmp -lpthread -std=c++11  -o TCGA2015.out TCGA2015.o NonogramSolver.o

TCGA2015.o: TCGA2015.cpp NonogramSolver.h
	g++-6 -fopenmp -std=c++11  -c TCGA2015.cpp 
	
NonogramSolver.o: NonogramSolver.h NonogramSolver.cpp NonogramSolver.tpp Line.tpp
	g++-6 -fopenmp -std=c++11  -c NonogramSolver.cpp

# NonogramSolver.o: NonogramSolver.h hope.cpp NonogramSolver.tpp Line.tpp
# 	g++-6 -fopenmp -std=c++11 -g -c hope.cpp -o NonogramSolver.o



clean:
	rm -f *.o
