#ifndef NONOGRAMSOLVER
#define NONOGRAMSOLVER

#include<vector>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string.h>
#include<iomanip>
using namespace std ;

typedef char State ;
const State empty = 2 ;
const State full = 1 ;
const State unknown = 0 ;

struct Range {
	Range(){
		cntBlack = 0 ;
	}
	int L ,R ; // [L,R)
	int cntBlack ,len ;
};

template<int cntRow ,int cntCol >
struct NonogramSolver {
	struct Record_type {
		Record_type(){
			LinePutToLimit = 0 ;
			AllPutToLimit = 0 ;
		}
		int LinePutToLimit ;
		int AllPutToLimit ;
	} ;
	Record_type Record ;

	template<int length>
	struct Line {
		vector<Range> left_most ,right_most ;
		Line(){
			memset(line,0,sizeof(line)) ;
		}

		State line[length+2] ;
		State& operator [] (const int id) {
			return line[id] ;
		}

		void init(){
			int n = left_most.size() ;

			{	//init left_most 
				auto &rang = left_most ;
				rang[0].L = 1 ;
				rang[0].R = rang[0].L + rang[0].len ;

				for (int i=1 ; i<n ;i++ ){
					rang[i].L = rang[i-1].R + 1 ;
					rang[i].R = rang[i].L + rang[i].len ;
				}
			}
			{	//init right_most 
				auto &rang = right_most ;
				rang[n-1].R = length+1 ;
				rang[n-1].L = rang[n-1].R - rang[n-1].len ;

				for (int i=n-2 ;i>=0 ;i-- ){
					rang[i].R = rang[i+1].L - 1 ;
					rang[i].L = rang[i].R - rang[i].len ;
				}
			}
		}
		void intersection(){
			for (int i=0 ;i<(int)left_most.size() ;i++ ){
				auto &ls = left_most ;
				auto &rs = right_most ;

				if (ls[i].L<=rs[i].L && ls[i].R>rs[i].L ){
					for (int j=rs[i].L ; j<ls[i].R ;j++ ){
						line[j] = full ;
					}
				}
			}
			bool cover[length+2] ;
			memset(cover,0,sizeof(cover)) ;
			auto &ls = left_most ;
			auto &rs = right_most ;
			for (int i=0 ;i<(int)left_most.size() ;i++ ){
				for (int j=ls[i].L ;j<rs[i].R ;j++ ){
					cover[j]=1 ;
				}	
			}
			for (int i=1 ;i<=length ;i++ ){
				if (cover[i]==0){
					line[i]=empty ;
				}
			}

		}
		void getCntBlack(Range &rang){
			rang.cntBlack=0 ;
			for (int i=rang.L ;i<rang.R ;i++ ){
				if (line[i]==empty)
					rang.cntBlack++ ;
			}
		}
		bool rangeOK(Range rang){
			getCntBlack(rang) ;
			if (rang.cntBlack>0) return 0 ;
			if (line[ rang.L-1 ]==full) return 0 ;
			if (line[ rang.R ]==full) return 0 ;
			return 1 ;
		}
		void putToLimit(){
			//Record.LinePutToLimit++ ;

			int n = left_most.size() ;

			{
				auto &ls = left_most ;
				int id=0 ,pos=ls[0].R ;

				while ( id<n && pos<=length ){
					if (id==0){
						ls[0].L = max(1,ls[0].L) ;
						ls[0].R = ls[0].L + ls[0].len ;
					}
					else {
						ls[id].L = max( ls[id-1].R+1 ,ls[id].L ) ;
						ls[id].R = ls[id].L + ls[id].len ;
					}
					if (id<n-1 && ls[id].R<pos && ls[id+1].L<=pos ){
						id++ ;
						continue ;
					}
					pos = max( pos , ls[id].R ) ;
					
					if (line[pos]==full || !rangeOK(ls[id]) ){
						int nextpos = ls[id].L ;
						ls[id].L++ ,ls[id].R++ ;
						if (id!=0){
							id-- ;
							pos = nextpos ;
						}
					}
					else {
						pos++ ;
					}
				}
			}
			{
				auto &rs = right_most ;
				int id=n-1 ,pos=rs[n-1].L-1 ;

				while (id>=0 && pos>=0 ){
					if (id==n-1){
						rs[n-1].R = min( length+1 , rs[n-1].R ) ;
						rs[n-1].L = rs[n-1].R - rs[n-1].len ;
					}
					else {
						rs[id].R = min( rs[id+1].L-1 ,rs[id].R ) ;
						rs[id].L = rs[id].R - rs[id].len ;
					}
					if (id>0 && pos<rs[id].L-1 && pos<rs[id-1].R ){
						id-- ;
						continue ;
					}
					pos = min( pos , rs[id].L-1 ) ;

					if (line[pos]==full || !rangeOK(rs[id]) ){
						int nextpos = rs[id].R-1 ;
						rs[id].L-- ,rs[id].R-- ;
						if (id!=n-1){
							id++ ;
							pos = nextpos ;
						}
					}
					else {
						pos-- ;
					}
				}
			}
		}
		
		bool LineOK(){
			if ( left_most[ left_most.size()-1 ].R>length+1 )
				return 0 ;
			for (int i=1 ;i<left_most[0].L ;i++ ){
				if (line[i]==full)return 0 ;
			}
		
			for (int i=0 ;i<(int)left_most.size() ;i++ ){
				if (left_most[i].L>right_most[i].L)
					return 0 ;
			}
			return 1 ;
		}
	};
	Line<cntRow> Col[cntCol+2] ;
	Line<cntCol> Row[cntRow+2] ;
	
	bool GridOK(){
		for (int i=1 ;i<=cntRow ;i++ )
			if (!Row[i].LineOK())return 0 ;
		for (int i=1 ;i<=cntCol ;i++ )
			if (!Col[i].LineOK())return 0 ;
		return 1 ;
	}

	void init(){
		//memset(Record,0,sizoeof(Record)) ;
		ans_outed=0 ;
		for (int i=1 ;i<=cntCol ;i++ )
			Col[i].init() ;
		for (int i=1 ;i<=cntRow ;i++ )
			Row[i].init() ;
	}
	
	void putToLimit(){
		Record.AllPutToLimit++ ;
		for (int i=1 ;i<=cntCol ;i++ )
			Col[i].putToLimit() ;
		for (int i=1 ;i<=cntRow ;i++ )
			Row[i].putToLimit() ;
	}
	
	void LogicSolve(){
		do {
			putToLimit() ;
		}while( intersection()==1 ) ;
	}

	bool Union(){
		bool updated=0 ;
		for (int i=1 ;i<=cntRow ;i++ ){
			for (int j=1 ;j<=cntCol ;j++ ){
				if (Row[i][j]==unknown && Col[j][i]!=unknown ) Row[i][j]=Col[j][i] ,updated=1 ;
				if (Col[j][i]==unknown && Row[i][j]!=unknown ) Col[j][i]=Row[i][j] ,updated=1 ;
			}
		}
		return updated ;
	}

	bool intersection(){
		for (int i=1 ;i<=cntCol ;i++ )
			Col[i].intersection() ;
		for (int i=1 ;i<=cntRow ;i++ )
			Row[i].intersection() ;
		return Union() ;
	}

	template<int ll>
	void inputRanges(ifstream &fin ,Line<ll> line[] ,int cntLine ){
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
	
	void input(string filename){
		ifstream fin(filename.c_str()) ;
		input(fin) ;
	}

	void input(ifstream &fin ){
		inputRanges(fin,Col,cntCol) ;
		inputRanges(fin,Row,cntRow) ;
		init() ;
	}

	void output(ostream &fout = cout){
		fout <<endl << cntCol <<" x " << cntRow <<endl ; 
		for (int i=1 ;i<=cntCol ;i++ ) fout << "  _" ; fout <<endl ;

		for (int i=1 ;i<=cntRow ;i++ ){
			for (int j=1 ;j<=cntCol ;j++ ){
				fout <<"  " ;
				if ( Row[i][j]==empty  )
					fout <<"X" ;
				else if ( Row[i][j]==full )
					fout <<"O" ;
				else 
					fout <<" " ;
			}
			for (auto r : Row[i].left_most ){
				fout << "|" << setw(3) << r.len <<" " ;
			}
			fout <<endl ;
		}
		
		for (int i=1 ;i<=cntCol ;i++ ) fout << "  _" ; fout <<endl ;

		for (int i=0 ; ;i++ ){
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

	bool finish(){
		for (int i=1 ;i<=cntRow ;i++ ){
			for (int j=1 ;j<=cntCol ;j++ ){
				if (Row[i][j]==unknown)return 0 ;
			}
		}
		return 1 ;
	}

	bool ans_outed ;
	void Solve(){
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
};

#endif
