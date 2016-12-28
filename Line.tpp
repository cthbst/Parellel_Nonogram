#include <string.h>

typedef char State;

template<int length>
Line<length>::Line()
{
	memset(line,0,sizeof(line)) ;
}

template<int length>
State& Line<length>::operator[] (const int id)
{
	return line[id];
}

template<int length>
void Line<length>::init()
{
	int n = left_most.size() ;

	{	
		//init left_most 
		auto &rang = left_most ;
		rang[0].L = 1 ;
		rang[0].R = rang[0].L + rang[0].len ;

		for (int i=1 ; i<n ;i++ ){
			rang[i].L = rang[i-1].R + 1 ;
			rang[i].R = rang[i].L + rang[i].len ;
		}
	}
	{	
		//init right_most 
		auto &rang = right_most ;
		rang[n-1].R = length+1 ;
		rang[n-1].L = rang[n-1].R - rang[n-1].len ;

		for (int i=n-2 ;i>=0 ;i-- ){
			rang[i].R = rang[i+1].L - 1 ;
			rang[i].L = rang[i].R - rang[i].len ;
		}
	}
}

template<int length>
void Line<length>::intersection()
{
	const auto &ls = left_most ;
	const auto &rs = right_most ;
	for (int i=0 ;i<(int)left_most.size() ;i++ )
	{
		if (ls[i].L<=rs[i].L && ls[i].R>rs[i].L )
		{
			for (int j=rs[i].L ; j<ls[i].R ;j++ )
			{
				line[j] = full ;
			}
		}
	}
	bool cover[length+2] ;
	memset(cover,0,sizeof(cover)) ;
	// const auto &ls = left_most ;
	// auto &rs = right_most ;
	for (int i=0 ;i<(int)left_most.size() ;i++ )
	{
		for (int j=ls[i].L ;j<rs[i].R ;j++ )
		{
			cover[j]=1 ;
		}	
	}
	for (int i=1 ;i<=length ;i++ )
	{
		if (cover[i]==0){
			line[i]=empty ;
		}
	}
}

template<int length>
void Line<length>::getCntBlack(Range &rang)
{
	rang.cntBlack=0 ;
	for (int i=rang.L ;i<rang.R ;i++ ){
		if (line[i]==empty)
			rang.cntBlack++ ;
	}
}

template<int length>
bool Line<length>::rangeOK(Range rang)
{
	getCntBlack(rang) ;
	if (rang.cntBlack>0) return 0 ;
	if (line[ rang.L-1 ]==full) return 0 ;
	if (line[ rang.R ]==full) return 0 ;
	return 1 ;
}

template<int length>
void Line<length>::putToLimit()
{
	const int n = left_most.size() ;

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

template<int length>
bool Line<length>::LineOK()
{
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



