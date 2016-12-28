

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iomanip>

#include <omp.h>

#define tnum 4
//#include "NonogramSolver.h"
using namespace std ;

typedef char State ;
// const State empty = 2 ;
// const State full = 1 ;
// const State unknown = 0 ;

static int l_count =0;



template<int cntRow ,int cntCol > 
bool NonogramSolver<cntRow,cntCol>::GridOK()
{
	for (int i=1 ;i<=cntRow ;i++ )
		if (!Row[i].LineOK())return 0 ;
	for (int i=1 ;i<=cntCol ;i++ )
		if (!Col[i].LineOK())return 0 ;
	return 1 ;
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::init()
{
		//memset(Record,0,sizoeof(Record)) ;
	ans_outed=0 ;
	#pragma omp parallel for num_threads(tnum)
	for (int i=1 ;i<=cntCol ;i++ )
		Col[i].init() ;
	#pragma omp parallel for num_threads(tnum)
	for (int i=1 ;i<=cntRow ;i++ )
		Row[i].init() ;
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::putToLimit()
{
	Record.AllPutToLimit++ ;
	//l_count++;
	int i;
	#pragma omp parallel for schedule(auto) num_threads(tnum) private(i)
	for (i=1 ;i<=cntCol ;i++ ){
		Col[i].putToLimit() ;
		Row[i].putToLimit() ;
	}

	/*#pragma omp parallel for  num_threads(tnum) private(i)
	for (i=1 ;i<=cntRow ;i++ )
		Row[i].putToLimit() ;
		*/
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::LogicSolve()
{
	do {
		putToLimit() ;
	}while( intersection()==1 ) ;
}

template<int cntRow ,int cntCol > 
bool NonogramSolver<cntRow,cntCol>::Union()
{
	bool Updated=0;
/*
	for (int i=1; i<=cntRow && !Updated ;i++ ){
		for (int j=1; j<=cntCol && !Updated ;j++ ){
			if (Row[i][j]==unknown && Col[j][i]!=unknown ) 
		 		Updated=1 ;
		 	if (Col[j][i]==unknown && Row[i][j]!=unknown ) 
		 		Updated=1 ;
		}
	}
*/	int i,j;
	//#pragma omp parallel for num_threads(tnum) private(i,j)
	for (i=1; i<=cntRow ;i++ ){
		for (j=1; j<=cntCol ;j++ ){
			if (Row[i][j]==unknown && Col[j][i]!=unknown ) 
				Row[i][j]=Col[j][i] ,Updated=1;
			else if (Col[j][i]==unknown && Row[i][j]!=unknown ) 
				Col[j][i]=Row[i][j] ,Updated=1;
		}
	}
	return Updated ;
}

template<int cntRow ,int cntCol > 
bool NonogramSolver<cntRow,cntCol>::intersection()
{
	int i;
	//#pragma omp parallel for num_threads(tnum) private(i)
	for (i=1 ;i<=cntCol ;i++ )
		Col[i].intersection() ;
	//#pragma omp parallel for num_threads(tnum) private(i)
	for (i=1 ;i<=cntRow ;i++ )
		Row[i].intersection() ;
	return Union() ;
}

template<int cntRow, int cntCol>
template<int ll>
void NonogramSolver<cntRow,cntCol>::inputRanges(ifstream &fin ,Line<ll> line[] ,int cntLine )
{
	for (int i=1 ;i<=cntLine ;i++ ){
		string in ;
		getline(fin,in) ;
		stringstream ss ; ss << in ;

		Range rang ;
		while (ss >> rang.len){
			line[i].left_most.push_back(rang) ;
			line[i].right_most.push_back(rang) ;
		}
	}
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::input(string filename)
{
	ifstream fin(filename.c_str()) ;
	input(fin) ;
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::input(ifstream &fin )
{
	inputRanges(fin,Col,cntCol) ;
	inputRanges(fin,Row,cntRow) ;
	init() ;
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::output(ostream &fout)
{
	fout <<endl << cntCol <<" x " << cntRow <<endl ; 
	for (int i=1 ;i<=cntCol ;i++ )
		fout << "  _" ; fout <<endl ;

	for (int i=1 ;i<=cntRow ;i++ )
	{
		for (int j=1 ;j<=cntCol ;j++ )
		{
			fout <<"  " ;
			if ( Row[i][j]==empty  )
				fout <<"X" ;
			else if ( Row[i][j]==full )
				fout <<"O" ;
			else 
				fout <<" " ;
		}
		for (auto r : Row[i].left_most )
		{
			fout << "|" << setw(3) << r.len <<" " ;
		}
		fout <<endl ;
	}

	for(int i=1 ;i<=cntCol ;i++ ) 
		fout << "  _" ; fout <<endl ;

	for (int i=0 ; ;i++ )
	{
		bool outed=0 ;
		for (int j=1 ;j<=cntCol ;j++ ){
			if ((int)Col[j].left_most.size()>i ){
				fout << setw(3) << Col[j].left_most[i].len ;
				outed=1 ;
			}
			else {
				fout << "   " ;
			}
		}
		fout <<endl ;

		if (outed==0)break ;
	}
	fout <<"LinePutToLimit : " << Record.LinePutToLimit <<endl ;
	fout <<"AllPutToLimit : " << Record.AllPutToLimit <<endl ;
	fout <<"GridOK : " << GridOK() <<endl ;
}

template<int cntRow ,int cntCol > 
bool NonogramSolver<cntRow,cntCol>::finish()
{
	for (int i=1 ;i<=cntRow ;i++ ){
		for (int j=1 ;j<=cntCol ;j++ ){
			if (Row[i][j]==unknown)return 0 ;
		}
	}
	return 1 ;
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::Solve(){
	if (ans_outed)
		return ;

	LogicSolve() ; // p

	if (!GridOK())
		return ;

	if (finish()){
		output() ;
		cout << "finish" <<endl ;
		cout << "lcount = " << l_count << endl;
		ans_outed=1 ;
		return ;
	}

		//output() ;
	//need p
	for (int i=1 ;i<=cntRow ;i++ ){
		for (int j=1 ;j<=cntCol ;j++ ){
			if (Row[i][j]==unknown){
				NonogramSolver A = *this ;
				A.Row[i][j]=full ;
				A.Solve() ;
				l_count++;

				if (A.ans_outed==1){
					ans_outed=1 ;
					return ;
				}
				A = *this ;
				A.Row[i][j]=empty ;
				A.Solve() ;
				l_count++;
				
				if (A.ans_outed==1){
					ans_outed=1 ;
					return ;
				}
				return ;
			}
		}
	}

}

