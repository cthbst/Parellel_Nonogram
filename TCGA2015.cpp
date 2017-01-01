#include "NonogramSolver.h"
#include "lib.hpp"

using namespace std;

#define size 25

int main(int argc,char* argv[]){
	ifstream fin(argv[1]) ;
	// ifstream fin("Input/sample_in.txt") ;

	clock_t start = clock() ;

	for (int i=1 ;i<=1 ;i++ ){
		string name ;
		getline(fin,name) ;

		//cout <<setw(4)<< i << " " ;
		NonogramSolver<size,size> Game ;
		Game.input(fin) ;

		elapsed_time();

		Game.Solve() ;

		cout << "time:" <<elapsed_time(0)<<endl;
		
		//cout << "\t" << (clock()-start)/(double)CLOCKS_PER_SEC <<" s" <<endl ;
	}
}
