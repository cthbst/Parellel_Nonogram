

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iomanip>
//#include "NonogramSolver.h"
using namespace std ;

typedef char State ;
// const State empty = 2 ;
// const State full = 1 ;
// const State unknown = 0 ;



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
	for (int i=1 ;i<=cntCol ;i++ )
		Col[i].init() ;
	for (int i=1 ;i<=cntRow ;i++ )
		Row[i].init() ;
}

template<int cntRow ,int cntCol > 
void NonogramSolver<cntRow,cntCol>::putToLimit()
{
	Record.AllPutToLimit++ ;
	for (int i=1 ;i<=cntCol ;i++ )
		Col[i].putToLimit() ;
	for (int i=1 ;i<=cntRow ;i++ )
		Row[i].putToLimit() ;
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
	bool updated=0 ;
	for (int i=1 ;i<=cntRow ;i++ ){
		for (int j=1 ;j<=cntCol ;j++ ){
			if (Row[i][j]==unknown && Col[j][i]!=unknown ) Row[i][j]=Col[j][i] ,updated=1 ;
			if (Col[j][i]==unknown && Row[i][j]!=unknown ) Col[j][i]=Row[i][j] ,updated=1 ;
		}
	}
	return updated ;
}

template<int cntRow ,int cntCol > 
bool NonogramSolver<cntRow,cntCol>::intersection()
{
	for (int i=1 ;i<=cntCol ;i++ )
		Col[i].intersection() ;
	for (int i=1 ;i<=cntRow ;i++ )
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
	if (ans_outed)return ;
	LogicSolve() ;
	if (!GridOK())return ;

	if (finish()){
		output() ;
		cout << "finish" <<endl ;
		ans_outed=1 ;
		return ;
	}

		//output() ;
	for (int i=1 ;i<=cntRow ;i++ ){
		for (int j=1 ;j<=cntCol ;j++ ){
			if (Row[i][j]==unknown){
				NonogramSolver A = *this ;
				A.Row[i][j]=full ;
				A.Solve() ;

				if (A.ans_outed==1){
					ans_outed=1 ;
					return ;
				}
				A = *this ;
				A.Row[i][j]=empty ;
				A.Solve() ;
				if (A.ans_outed==1){
					ans_outed=1 ;
					return ;
				}
				return ;
			}
		}
	}
}
