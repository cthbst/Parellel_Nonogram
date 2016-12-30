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

void unrecursive_solver()
{
printf("%d\n",stk_nono.size());
	restart:

	while (!stk_nono.empty())
	{
		//printf("aa\n");
		NonogramSolver<25,25> solverA=stk_nono.top();
		stk_nono.pop();

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
			while(!stk_nono.empty())
			{
				stk_nono.pop();
			}

			solverA.output();
			cout << "finish" << endl;
			solverA.ans_outed=1;
			return;
		}

		for (int i=1 ;i<=25 ;i++ ){
			for (int j=1 ;j<=25 ;j++ ){
				if (solverA.Row[i][j]==unknown){


					solverA.Row[i][j]=full;
					stk_nono.push(solverA);

					solverA.Row[i][j]=empty;
					stk_nono.push(solverA);

					goto restart;
				}
			}
		}

	}

}



// #endif