#include "NonogramSolver.h"

#include<stdio.h>

int main(){
	NonogramSolver<25,25> Game ;
	//Game.input("sample_in_2.txt") ; 
	//Game.input("tcga2015-question.txt") ;

	ifstream TCGA2015("tcga2015-question.txt") ;
	for (int i=1 ;i<=1000 ;i++ ){		
		string QuestionNum ;
		getline(TCGA2015,QuestionNum) ;
		cout << "----------------------------------------------" ;
		cout <<endl << QuestionNum <<endl ;

		NonogramSolver<25,25> GG ;
		GG.input(TCGA2015) ;
		GG.init() ;
		GG.LogicSolve() ;
		GG.output() ;
	}

	Game.init() ;
	Game.LogicSolve() ;
	Game.output() ;
}

