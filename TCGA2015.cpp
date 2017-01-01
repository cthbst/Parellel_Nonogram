#include "NonogramSolver.h"
#include "lib.hpp"
#include <pthread.h>

using namespace std;

#define nsize 25




int main(int argc, char **argv){
	if (argc!=3){
		printf("%d\n",argc);
		puts("need 2 argv");
		return 0;
	}
	set_thread_n( strtol(argv[2],NULL,10) );
	ifstream fin(argv[1]) ;
	//ifstream fin("Input/sample_in.txt") ;
	clock_t start = clock() ;

	pthread_t* handles=NULL;
	handles=(pthread_t*)malloc(get_thread_n() * sizeof(pthread_t) );

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



		elapsed_time();

		for(int t=0;t<get_thread_n();t++)
		{
			//printf("%d\n",t);
			rank[t]=t;
			pthread_create(&handles[t],NULL,unrecursive_solver,&rank[t]);
		}

		for(int t=0;t<get_thread_n();t++)
		{
			pthread_join(handles[t],NULL);
		}

		cout << "time:" <<elapsed_time(0)<<endl;

		//p_outans();
		//unrecursive_solver();
		//cout << "\t" << (clock()-start)/(double)CLOCKS_PER_SEC <<" s" <<endl ;
	}
	mu_des();
}
