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
pthread_mutex_t mu_check;

void mu_init()
{
	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&mu_check,NULL);
}
void mu_des()
{
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mu_check);
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
	int is_empty;

	restart:

	pthread_mutex_lock(&mutex);
	{
		is_empty=stk_nono.empty();
	}
	pthread_mutex_unlock(&mutex);


	pthread_mutex_lock(&mu_check);
	{
		if(!is_conti)
		{
			pthread_mutex_unlock(&mu_check);
			return NULL;
		}
	}
	pthread_mutex_unlock(&mu_check);


	if(is_empty)
	{	
		goto restart;
	}

	pthread_mutex_lock(&mutex);
	
	NonogramSolver<25,25> solverA=stk_nono.top();
	stk_nono.pop();
	
	pthread_mutex_unlock(&mutex);


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
		pthread_mutex_lock(&mu_check);
		{
			is_conti=0;
			outans=solverA;
		}
		pthread_mutex_unlock(&mu_check);

		pthread_mutex_lock(&mutex);
		{
			while(!stk_nono.empty())
			{
				stk_nono.pop();
			}
		}
		pthread_mutex_unlock(&mutex);

		return NULL;
	}

	for (int i=1 ;i<=25 ;i++ ){
		for (int j=1 ;j<=25 ;j++ ){
			if (solverA.Row[i][j]==unknown){


				pthread_mutex_lock(&mutex);
				{
					solverA.Row[i][j]=full;
					stk_nono.push(solverA);

					solverA.Row[i][j]=empty;
					stk_nono.push(solverA);
				}
				pthread_mutex_unlock(&mutex);

				goto restart;
			}
		}
	}

	goto restart;

}



// #endif