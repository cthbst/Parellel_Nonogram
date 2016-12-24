

TCGA2015: TCGA2015.o NonogramSolver.o
	g++-6 -o TCGA2015.out nonogram_solver.o NonogramSolver.o

nonogram_solver.o: TCGA2015.cpp NonogramSolver.h
	g++-6 -c TCGA2015.cpp

NonogramSolver.o: NonogramSolver.h NonogramSolver.cpp NonogramSolver.hpp
	g++-6 -c NonogramSolver.cpp