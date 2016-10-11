#include "NonogramSolver.h"	

int main(){
	NonogramSolver<25,25> Game ;
	Game.input("Input/sample_in_3.txt") ;
	Game.Solve() ;
	cout << Game.GridOK() ;
}
