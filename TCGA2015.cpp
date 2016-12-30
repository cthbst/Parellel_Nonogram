#include "NonogramSolver.h"

using namespace std;

#define nsize 25

int main(){
	ifstream fin("Input/sample_in.txt") ;

	clock_t start = clock() ;

	for (int i=1 ;i<=1 ;i++ ){
		string name ;
		getline(fin,name) ;

		cout <<setw(4)<< i << " " ;
		NonogramSolver<nsize,nsize> Game ;
		Game.input(fin) ;
		//Game.Solve() ;
		Game.output();
		push_stk(Game);
		//printf("%d\n",stk_nono.size());

		unrecursive_solver();
		cout << "\t" << (clock()-start)/(double)CLOCKS_PER_SEC <<" s" <<endl ;
	}
}
