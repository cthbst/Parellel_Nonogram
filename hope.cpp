// #ifndef NONOGRAMSOLVER
// #define NONOGRAMSOLVER

// #include <vector>
// #include <iostream>
// #include <sstream>
// #include <fstream>
// #include <string.h>
// #include <iomanip>
#include "NonogramSolver.h"
using namespace std ;

typedef char State ;
// const State empty = 2 ;
// const State full = 1 ;
// const State unknown = 0 ;

stack< NonogramSolver<25,25> > stk_nono;
pthread_mutex_t mutex;

void mu_init()
{
	pthread_mutex_init(&mutex,NULL);
}
void mu_des()
{
	pthread_mutex_destroy(&mutex);
}

void push_stk(NonogramSolver<25,25> A)
{
	stk_nono.push(A);
}

Range::Range()
{
	cntBlack=0;
}

Record_type::Record_type()
{
	LinePutToLimit = 0;
	AllPutToLimit = 0;
}

int is_conti=1;
NonogramSolver<25,25> outans;

void p_outans()
{
	outans.output();
}

void* unrecursive_solver(void*)
{
	restart:

	pthread_mutex_lock(&mutex);
	int check=stk_nono.empty();
	


	while (!check || is_conti)
	{
		//printf("aa\n");
		if(check)
		{	
			puts("Error should not show this!!");
			pthread_mutex_unlock(&mutex);
			goto restart;
		}
		//pthread_mutex_unlock(&mutex);


		//pthread_mutex_lock(&mutex);
		NonogramSolver<25,25> solverA=stk_nono.top();
		stk_nono.pop();
		int have_lock = 0;
		if (stk_nono.empty()){
			have_lock=1;
		}
		else {
			pthread_mutex_unlock(&mutex);
		}

		if(solverA.ans_outed)
		{
			goto restart;
		}

		solverA.LogicSolve();

		if(!solverA.GridOK())
		{
			goto restart;
		}

		// need tag for finish !!!!
		if(solverA.finish())
		{
			is_conti=0;

			
			if (!have_lock){
				pthread_mutex_lock(&mutex);
			}
			outans=solverA;
			while(!stk_nono.empty())
			{
				stk_nono.pop();
			}
			

			//solverA.output();
			//cout << "finish" << endl;
			//solverA.ans_outed=1;
			pthread_mutex_unlock(&mutex);
			return NULL;
		}

		for (int i=1 ;i<=25 ;i++ ){
			for (int j=1 ;j<=25 ;j++ ){
				if (solverA.Row[i][j]==unknown){
					if (!have_lock){
						pthread_mutex_lock(&mutex);
					}
					solverA.Row[i][j]=full;
					stk_nono.push(solverA);

					solverA.Row[i][j]=empty;
					stk_nono.push(solverA);
					pthread_mutex_unlock(&mutex);

					goto restart;
				}
			}
		}

		if (!have_lock){
			pthread_mutex_lock(&mutex);
		}
		check=stk_nono.empty();
		pthread_mutex_unlock(&mutex);
	}
	

}



// #endif