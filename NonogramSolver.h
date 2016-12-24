#ifndef NONOGRAMSOLVER
#define NONOGRAMSOLVER

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iomanip>

using namespace std;


typedef char State ;
const State empty = 2 ;
const State full = 1 ;
const State unknown = 0 ;


struct Range 
{
	Range();
	int L ,R ; // [L,R)
	int cntBlack ,len ;
};



template<int cntRow ,int cntCol > struct NonogramSolver
{
	struct Record_type 
	{
		Record_type();
		int LinePutToLimit ;
		int AllPutToLimit ;
	} ;

	Record_type Record ;

	template<int length>struct Line 
	{
		vector<Range> left_most ,right_most ;
		Line();


		State line[length+2];
		State& operator [] (const int id);

		void init();
		void intersection();
		void getCntBlack(Range &rang);
		bool rangeOK(Range rang);
		void putToLimit();

		bool LineOK();

	};

	Line<cntRow> Col[cntCol+2] ;
	Line<cntCol> Row[cntRow+2] ;

	bool GridOK();
	void init();
	void putToLimit();
	void LogicSolve();
	bool Union();
	bool intersection();
					template<int ll>void inputRanges(ifstream &fin ,Line<ll> line[] ,int cntLine );
	void input(string filename);
	void input(ifstream &fin );

	void output(ostream &fout = cout);

	bool finish();

	bool ans_outed ;
	void Solve();
};

#include "NonogramSolver.hpp"

#endif
