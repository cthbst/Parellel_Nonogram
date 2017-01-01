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
pthread_mutex_t mu_empt;
pthread_mutex_t mu_busy;
int cnt_busy;

int thread_n=4;
void set_thread_n(int _n){
	thread_n = _n;
}

int get_thread_n(){
	return thread_n;
}

void mu_init()
{
	pthread_mutex_init(&mu_take,NULL);
	pthread_mutex_init(&mu_push,NULL);
	pthread_mutex_init(&mu_empt,NULL);
	pthread_mutex_init(&mu_busy,NULL);
	cnt_busy=0;
}
void mu_des()
{
	pthread_mutex_destroy(&mu_take);
	pthread_mutex_destroy(&mu_push);
	pthread_mutex_destroy(&mu_empt);
	pthread_mutex_destroy(&mu_busy);
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

int is_conti=1;  // when find out answer or no solution ,is_conti=0
NonogramSolver<25,25> outans;

void p_outans()
{
	if (!outans.ans_outed){
		puts("No Solution");
	}
	else {
		outans.output();
	}
}



void* unrecursive_solver(void* a)
{
	int* rank=(int*)a;
	int is_empty;
	NonogramSolver<25,25> solverA;

	restart:

	//不用繼續就跳掉
	pthread_mutex_lock(&mu_take);					// mutex mu_take lock
	
	if(!is_conti)
	{
		pthread_mutex_unlock(&mu_take);				// mutex mu_take unlock
		return NULL;
	}
	pthread_mutex_unlock(&mu_take);					// mutex mu_take unlock

	//如果堆疊空 等到堆疊不空為止
	//否則取物件 
	//一次只有一個人
	pthread_mutex_lock(&mu_busy);					// mutex mu_busy lock
	cnt_busy++;
	pthread_mutex_unlock(&mu_busy);					// mutex mu_busy unlock
	pthread_mutex_lock(&mu_empt);					// mutex mu_empt lock

	reempty:
	if(stk_nono.empty())
	{	
		pthread_mutex_lock(&mu_busy);				// mutex mu_busy lock
		if (cnt_busy==thread_n){
			pthread_mutex_unlock(&mu_busy);			// mutex mu_busy unlock
			pthread_mutex_unlock(&mu_empt);			// mutex mu_empt unlock
			return NULL;
		}
		pthread_mutex_unlock(&mu_busy);				// mutex mu_busy unlock
		goto reempty;
	}
	else
	{
		pthread_mutex_lock(&mu_push);				// mutex mu_push lock
		pthread_mutex_lock(&mu_busy);				// mutex mu_busy lock
		cnt_busy--;
		pthread_mutex_unlock(&mu_busy);				// mutex mu_busy unlock
		solverA=stk_nono.top();
		stk_nono.pop();	
		pthread_mutex_unlock(&mu_push);				// mutex mu_push unlock
	}
	pthread_mutex_unlock(&mu_empt);					// mutex mu_empt unlock
	
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
		pthread_mutex_lock(&mu_take);				// mutex mu_take lock
		is_conti=0;
		outans=solverA;
		outans.ans_outed=1;
		pthread_mutex_unlock(&mu_take);				// mutex mu_take unlock

		//清空堆疊 或許不用
		// while(!stk_nono.empty())
		// {
		// 	stk_nono.pop();
		// }

		return NULL;
	}

	//如果有其中一個 unknown
	for (int i=1 ;i<=25 ;i++ ){
		for (int j=1 ;j<=25 ;j++ ){
			if (solverA.Row[i][j]==unknown){

				//猜測黑白 丟到堆疊做
				pthread_mutex_lock(&mu_push);		// mutex mu_push lock
				solverA.Row[i][j]=full;
				stk_nono.push(solverA);

				solverA.Row[i][j]=empty;
				stk_nono.push(solverA);
				pthread_mutex_unlock(&mu_push);		// mutex mu_push unlock

				goto restart;
			}
		}
	}

	goto restart;

}



// #endif
