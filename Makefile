

nonogram_solver: nonogram_solver.o NonogramSolver.o
	g++-6 -o main1.out nonogram_solver.o NonogramSolver.o

nonogram_solver.o: nonogram_solver.cpp
	g++-6 -c nonogram_solver.cpp

NonogramSolver.o: NonogramSolver.h NonogramSolver.cpp
	g++-6 -c NonogramSolver.cpp