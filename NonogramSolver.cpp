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
pthread_mutex_t mu_take;
pthread_mutex_t mu_push;

void mu_init()
{
	pthread_mutex_init(&mu_take,NULL);
	pthread_mutex_init(&mu_push,NULL);
}
void mu_des()
{
	pthread_mutex_destroy(&mu_take);
	pthread_mutex_destroy(&mu_push);
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
	NonogramSolver<25,25> solverA

	restart:

	//不用繼續就跳掉
	pthread_mutex_lock(&mu_take);
	if(!is_conti)
	{
		return NULL;
	}
	pthread_mutex_unlock(&mu_take);

	//如果堆疊空 等到堆疊不空為止
	//否則取物件 
	//一次只有一個人
	pthread_mutex_lock(&mu_take);
	reempty:
	if(stk_nono.empty())
	{	
		goto reempty;
	}
	else
	{
		solverA=stk_nono.top();
		stk_nono.pop();	
	}
	
	//不知道幹嘛 反正就回去
	if(solverA.ans_outed)
	{
		goto restart;
	}

	//解答案
	solverA.LogicSolve();

	//可能是解錯？ 回去
	if(!solverA.GridOK())
	{
		goto restart;
	}

		// need tag for finish !!!!
	//如果有答案了
	if(solverA.finish())
	{
		//不用繼續跑了 輸出答案
		is_conti=0;
		outans=solverA;

		//清空堆疊 或許不用
		while(!stk_nono.empty())
		{
			stk_nono.pop();
		}

		return NULL;
	}

	//如果有其中一個 unknown
	for (int i=1 ;i<=25 ;i++ ){
		for (int j=1 ;j<=25 ;j++ ){
			if (solverA.Row[i][j]==unknown){

				//猜測黑白 丟到堆疊做
				solverA.Row[i][j]=full;
				stk_nono.push(solverA);

				solverA.Row[i][j]=empty;
				stk_nono.push(solverA);

				goto restart;
			}
		}
	}

	goto restart;

}



// #endif