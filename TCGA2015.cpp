#include "NonogramSolver.h"

int main(){
	ifstream fin("Input/tcga2015-question.txt") ;

	clock_t start = clock() ;

	for (int i=1 ;i<=1000 ;i++ ){
		string name ;
		getline(fin,name) ;

		cout <<setw(4)<< i << " " ;
		NonogramSolver<25,25> Game ;
		Game.input(fin) ;
		Game.Solve() ;
		cout << "\t" << (clock()-start)/(double)CLOCKS_PER_SEC <<" s" <<endl ;
	}
}
