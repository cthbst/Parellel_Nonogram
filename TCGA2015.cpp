#include "NonogramSolver.h"
#include <pthread.h>

using namespace std;

#define nsize 25




int main(){
	ifstream fin("Input/sample_NoSolution.txt") ;
	//ifstream fin("Input/sample_in.txt") ;

	clock_t start = clock() ;

	pthread_t* handles=NULL;
	handles=(pthread_t*)malloc(thread_n * sizeof(pthread_t) );

	mu_init();
	int rank[100];
	for (int i=1 ;i<=1 ;i++ ){
		
		string name ;
		getline(fin,name) ;

		//cout <<setw(4)<< i << " " ;
		NonogramSolver<nsize,nsize> Game ;
		Game.input(fin) ;
		//Game.Solve() ;
		//Game.output();
		push_stk(Game);
		//printf("%d\n",stk_nono.size());

		for(int t=0;t<thread_n;t++)
		{
			//printf("%d\n",t);
			rank[t]=t;
			pthread_create(&handles[t],NULL,unrecursive_solver,&rank[t]);
		}

		for(int t=0;t<thread_n;t++)
		{
			pthread_join(handles[t],NULL);
		}

		p_outans();
		//unrecursive_solver();
		//cout << "\t" << (clock()-start)/(double)CLOCKS_PER_SEC <<" s" <<endl ;
	}
	mu_des();
}
